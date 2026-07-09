#include "graphboard/runtime/interpreter.hpp"

#include <cassert>
#include <string>
#include <vector>

using namespace graphboard::runtime;

namespace {

// A Host that records the exact sequence of builtin and component calls, so a
// test can assert what a page did when driven headless.
class RecordingHost : public Host {
public:
    struct Call {
        bool builtin = true;
        std::string component;
        std::string name;
        std::vector<Value> args;
    };
    std::vector<Call> calls;

    Value callBuiltin(const std::string& name, const std::vector<Value>& args) override {
        calls.push_back({true, "", name, args});
        // A couple of builtins have return values scripts branch on.
        if (name == "IsProject") return Value::integer(1);
        if (name == "Random") return Value::integer(0);
        return Value();
    }
    Value callComponent(const std::string& component, const std::string& method,
                        const std::vector<Value>& args) override {
        calls.push_back({false, component, method, args});
        return Value();
    }

    std::string trace() const {
        std::string out;
        for (const auto& c : calls) {
            if (!c.builtin) out += c.component + ".";
            out += c.name + "(";
            for (std::size_t i = 0; i < c.args.size(); ++i) {
                if (i) out += ",";
                out += c.args[i].toString();
            }
            out += ") ";
        }
        return out;
    }
};

// The real recovered INTRO.BDF page script.
const char* kIntroScript = R"SCRIPT(
OnOpenPage()
{
   LoadGroup("cursors.grp");
   SetCursor(1);
   FadeScreen(1,1000);
   Transparent_Video_Holder.Play(0,0);
   Sound_Holder.PlayDSound(0);
}

void OnClosePage(void)
{
}

void Transparent_Video_Holder.TheEnd(int videoID)
{
   if(videoID==9)
   {
      FadeScreen(0,1000);
      LoadPage("wyborw.bdf");
      return;
   }
   Transparent_Video_Holder.Play(videoID+1,0);
}

void OnLButtonDown(int x,int y)
{
   FadeScreen(0,1000);
   LoadPage("wyborw.bdf");
}
)SCRIPT";

void testDriveIntroOnOpenPage() {
    RecordingHost host;
    Interpreter interp(kIntroScript, host);

    assert(interp.hasHandler("OnOpenPage"));
    assert(interp.hasHandler("Transparent_Video_Holder.TheEnd"));

    interp.runHandler("OnOpenPage", {});
    assert(host.trace() ==
           "LoadGroup(cursors.grp) SetCursor(1) FadeScreen(1,1000) "
           "Transparent_Video_Holder.Play(0,0) Sound_Holder.PlayDSound(0) ");
}

void testTheEndBranches() {
    // videoID != 9 -> advances to the next clip.
    {
        RecordingHost host;
        Interpreter interp(kIntroScript, host);
        interp.runHandler("Transparent_Video_Holder.TheEnd", {Value::integer(3)});
        assert(host.trace() == "Transparent_Video_Holder.Play(4,0) ");
    }
    // videoID == 9 -> fade out and load the menu page, then return (no Play).
    {
        RecordingHost host;
        Interpreter interp(kIntroScript, host);
        interp.runHandler("Transparent_Video_Holder.TheEnd", {Value::integer(9)});
        assert(host.trace() == "FadeScreen(0,1000) LoadPage(wyborw.bdf) ");
    }
}

void testClickLoadsMenu() {
    RecordingHost host;
    Interpreter interp(kIntroScript, host);
    interp.runHandler("OnLButtonDown", {Value::integer(100), Value::integer(200)});
    assert(host.trace() == "FadeScreen(0,1000) LoadPage(wyborw.bdf) ");
}

// Exercise control flow, locals, user functions, arithmetic and switch beyond
// the INTRO sample.
void testControlFlowAndUserFunctions() {
    const char* script = R"S(
int total;

int square(int n)
{
   return n*n;
}

void OnTimer(int id)
{
   int i=0;
   total=0;
   while(i<4)
   {
      total = total + square(i);
      i=i+1;
   }
   Debug(total);          // 0+1+4+9 = 14

   switch(id)
   {
      case 1:
         Muzyka("a");
         break;
      case 2:
         Muzyka("b");
         break;
      default:
         Muzyka("z");
   }
}

void Muzyka(CString name)
{
   PlayTune(name);
}
)S";

    RecordingHost host;
    Interpreter interp(script, host);

    interp.runHandler("OnTimer", {Value::integer(2)});
    assert(interp.getGlobal("total").toInt() == 14);
    // Debug(14) from the loop, then Muzyka(2)->PlayTune("b") via the user fn.
    assert(host.trace() == "Debug(14) PlayTune(b) ");

    host.calls.clear();
    interp.runHandler("OnTimer", {Value::integer(7)});  // default case
    assert(host.trace() == "Debug(14) PlayTune(z) ");
}

// Regression for the ABECADLO hang: ++ / -- must actually mutate the loop
// counter, and a chained/array-heavy body must terminate and iterate correctly.
void testIncrementLoops() {
    const char* script = R"S(
void OnOpenPage()
{
   int puzz[4];
   int i=0;
   while(i<3)
   {
      Group.Sprite_Holder.ChangePhase(i,0);   // double-dotted member call
      puzz[i]=i;                               // array assign (unmodeled, no-op)
      i++;                                     // postfix
   }
   int j=5;
   --j;                                        // prefix
   Debug(j);                                   // 4
   int k=i++ + 1;                              // postfix in expression: i=3 -> 4, k=4
   Debug(k);
   Debug(i);                                   // 4
}
)S";

    RecordingHost host;
    Interpreter interp(script, host);
    interp.runHandler("OnOpenPage", {});

    // Three loop iterations (double-dotted component path preserved), then
    // Debug(4), Debug(4), Debug(4).
    assert(host.trace() ==
           "Group.Sprite_Holder.ChangePhase(0,0) Group.Sprite_Holder.ChangePhase(1,0) "
           "Group.Sprite_Holder.ChangePhase(2,0) Debug(4) Debug(4) Debug(4) ");
}

} // namespace

int main() {
    testDriveIntroOnOpenPage();
    testTheEndBranches();
    testClickLoadsMenu();
    testControlFlowAndUserFunctions();
    testIncrementLoops();
    return 0;
}
