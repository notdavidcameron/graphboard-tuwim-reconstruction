#include "graphboard/runtime/interpreter.hpp"

#include <cassert>
#include <cmath>
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
    ComponentResult callComponent(const std::string& component, const std::string& method,
                                  const std::vector<Value>& args) override {
        calls.push_back({false, component, method, args});
        ComponentResult result;
        if (method == "GetPosition") {
            result.outArguments[1] = Value::integer(12);
            result.outArguments[2] = Value::integer(34);
            result.outArguments[3] = Value::integer(56);
        } else if (method == "GetDeep") {
            result.outArguments[1] = Value::integer(78);
        }
        return result;
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
      puzz[i]=i;                               // array assign
      i++;                                     // postfix
   }
   int j=5;
   --j;                                        // prefix
   Debug(j);                                   // 4
   int k=i++ + 1;                              // postfix in expression: i=3 -> 4, k=4
   Debug(k);
   Debug(i);                                   // 4
   Debug(puzz[2]);                             // indexed read -> 2
}
)S";

    RecordingHost host;
    Interpreter interp(script, host);
    interp.runHandler("OnOpenPage", {});

    // Three loop iterations (double-dotted component path preserved), then
    // Debug(4), Debug(4), Debug(4), and the stored array element.
    assert(host.trace() ==
           "Group.Sprite_Holder.ChangePhase(0,0) Group.Sprite_Holder.ChangePhase(1,0) "
           "Group.Sprite_Holder.ChangePhase(2,0) Debug(4) Debug(4) Debug(4) Debug(2) ");
}

// Recovered engine rule (the scripts' own comment): every variable declared in
// OnOpenPage is a page-global visible to later handlers/callbacks. A variable
// declared in another handler stays local, and a user function called from
// OnOpenPage still gets ordinary locals (the promotion must not leak into it).
void testOnOpenPageGlobals() {
    const char* script = R"S(
void init(int seed)
{
   int scratch=seed+1;    // must NOT become a global despite being reached
   Debug(scratch);        // from inside OnOpenPage
}

void OnOpenPage()
{
   int ready=1;           // page-global per the engine
   int guarded=0;
   init(41);
}

void OnLButtonDown(int x,int y)
{
   int localOnly=7;       // stays local to this handler
   if(ready==1)   Sound_Holder.PlayDSound(guarded);
}
)S";

    RecordingHost host;
    Interpreter interp(script, host);

    interp.runHandler("OnOpenPage", {});
    // OnOpenPage's declarations were promoted to page-globals...
    assert(interp.hasGlobal("ready"));
    assert(interp.getGlobal("ready").toInt() == 1);
    assert(interp.hasGlobal("guarded"));
    // ...but the user function's local did not leak into globals.
    assert(!interp.hasGlobal("scratch"));
    assert(host.trace() == "Debug(42) ");

    // A later handler sees the promoted globals; its own decl stays local.
    host.calls.clear();
    interp.runHandler("OnLButtonDown", {Value::integer(0), Value::integer(0)});
    assert(host.trace() == "Sound_Holder.PlayDSound(0) ");
    assert(!interp.hasGlobal("localOnly"));
}

void testComponentOutParameters() {
    const char* script = R"S(
void OnOpenPage()
{
   int x=0; int y=0; int z=0; int deep=0;
}
void Query()
{
   Sprite_Holder.GetPosition(4,&x,&y,&z);
   Transparent_Video_Holder.GetDeep(2,&deep);
   Debug(x); Debug(y); Debug(z); Debug(deep);
}
)S";
    RecordingHost host;
    Interpreter interp(script, host);
    interp.runHandler("OnOpenPage", {});
    interp.runHandler("Query", {});
    assert(interp.getGlobal("x").toInt() == 12);
    assert(interp.getGlobal("y").toInt() == 34);
    assert(interp.getGlobal("z").toInt() == 56);
    assert(interp.getGlobal("deep").toInt() == 78);
    assert(host.trace() ==
           "Sprite_Holder.GetPosition(4,0,0,0) "
           "Transparent_Video_Holder.GetDeep(2,0) Debug(12) Debug(34) Debug(56) Debug(78) ");
}

void testCStringNavigationHelpers() {
    const char* script = R"S(
void OnOpenPage()
{
   int click=2;
   CString str;
   str.Format("mich%d.bdf",click+1);
   LoadPage(str);
   CString parent;
   parent.SetString("rycerz.bdf");
   Debug(parent);
}
)S";
    RecordingHost host;
    Interpreter interp(script, host);
    interp.runHandler("OnOpenPage", {});
    assert(host.trace() == "LoadPage(mich3.bdf) Debug(rycerz.bdf) ");
}

void testBrzechwaRectStringAndRealValues() {
    const char* script = R"S(
void OnOpenPage()
{
   int licznik=1;
   CRect filmy[3];
   filmy[0].SetRect(438,134,0,0);
   filmy[1].SetRect(341,129,0,0);
   filmy[licznik].right=7;
   filmy[licznik].right+=2;

   float prog=1.15*50;
   CString base;
   base.SetString("take");
   CString wav;
   wav.Format("%s_%d%%",base.GetString(),3);

   Debug(filmy[licznik].left);
   Debug(filmy[licznik].top);
   Debug(filmy[licznik].right);
   Debug(prog);
   Debug(wav);
   Debug(base.GetLength());
   base.Empty();
   Debug(base.GetLength());
   Group.Transparent_Video_Holder.MoveTo(licznik,filmy[licznik].left,filmy[licznik].top);
}
)S";

    RecordingHost host;
    Interpreter interp(script, host);
    interp.runHandler("OnOpenPage", {});
    assert(interp.getGlobal("filmy[1].left").toInt() == 341);
    assert(interp.getGlobal("filmy[1].right").toInt() == 9);
    assert(interp.getGlobal("prog").isReal());
    assert(std::abs(interp.getGlobal("prog").toDouble() - 57.5) < 0.000001);
    assert(interp.getGlobal("wav").toString() == "take_3%");
    assert(interp.getGlobal("base").toString().empty());
    assert(host.trace() ==
           "Debug(341) Debug(129) Debug(9) Debug(57.5) Debug(take_3%) "
           "Debug(4) Debug(0) Group.Transparent_Video_Holder.MoveTo(1,341,129) ");

    const Value value = Value::real(1.5);
    assert(value.isReal() && value.isNumeric());
    assert(value.toInt() == 1 && std::abs(value.toDouble() - 1.5) < 0.000001);
}

} // namespace

int main() {
    testDriveIntroOnOpenPage();
    testTheEndBranches();
    testClickLoadsMenu();
    testControlFlowAndUserFunctions();
    testIncrementLoops();
    testOnOpenPageGlobals();
    testComponentOutParameters();
    testCStringNavigationHelpers();
    testBrzechwaRectStringAndRealValues();
    return 0;
}
