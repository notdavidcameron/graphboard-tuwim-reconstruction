#include "graphboard/runtime/lexer.hpp"
#include "graphboard/runtime/script.hpp"

#include <cassert>
#include <string>

using namespace graphboard::runtime;

namespace {

// The INTRO.BDF page script, verbatim (the real recovered source), exercising
// page events, a dotted component callback, comments, strings and nesting.
const char* kIntroScript = R"SCRIPT(//###########################################################################
//Script code
//###########################################################################


OnOpenPage()
{
	// Add your component and variables initialization code here
   LoadGroup("cursors.grp");
   SetCursor(1);
   FadeScreen(1,1000);
   Transparent_Video_Holder.Play(0,0);
   Sound_Holder.PlayDSound(0);
}


void OnClosePage(void)
{
	// Add your close page handler code here
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

void testLexerBasics() {
    const auto tokens = tokenize("LoadGroup(\"cursors.grp\"); x==9 // trailing\n");
    // LoadGroup ( "cursors.grp" ) ; x == 9 End
    assert(tokens.size() == 9);
    assert(tokens[0].kind == TokenKind::Identifier && tokens[0].text == "LoadGroup");
    assert(tokens[1].kind == TokenKind::Punctuator && tokens[1].text == "(");
    assert(tokens[2].kind == TokenKind::String && tokens[2].text == "cursors.grp");
    assert(tokens[3].text == ")");
    assert(tokens[4].text == ";");
    assert(tokens[5].kind == TokenKind::Identifier && tokens[5].text == "x");
    assert(tokens[6].kind == TokenKind::Punctuator && tokens[6].text == "==");
    assert(tokens[7].kind == TokenKind::Number && tokens[7].text == "9");
    assert(tokens.back().kind == TokenKind::End);
}

void testLexerCommentsAndStrings() {
    // Block comment and an escaped quote inside a string.
    const auto tokens = tokenize("a /* b ( ) { } */ \"he said \\\"hi\\\"\" z");
    assert(tokens[0].text == "a");
    assert(tokens[1].kind == TokenKind::String);
    assert(tokens[1].text == "he said \\\"hi\\\"");
    assert(tokens[2].text == "z");
    assert(tokens[3].kind == TokenKind::End);
}

void testDiscoverIntroHandlers() {
    const std::string source = kIntroScript;
    const auto handlers = discoverHandlers(source);
    assert(handlers.size() == 4);

    assert(handlers[0].name == "OnOpenPage");
    assert(handlers[0].kind == HandlerKind::PageEvent);
    assert(handlers[0].returnType.empty());
    assert(handlers[0].parameters.empty());

    assert(handlers[1].name == "OnClosePage");
    assert(handlers[1].kind == HandlerKind::PageEvent);
    assert(handlers[1].returnType == "void");
    assert(handlers[1].parameters.empty());  // "(void)" -> no params

    assert(handlers[2].name == "Transparent_Video_Holder.TheEnd");
    assert(handlers[2].kind == HandlerKind::ComponentCallback);
    assert(handlers[2].component == "Transparent_Video_Holder");
    assert(handlers[2].method == "TheEnd");
    assert(handlers[2].parameters.size() == 1);
    assert(handlers[2].parameters[0].type == "int");
    assert(handlers[2].parameters[0].name == "videoID");

    assert(handlers[3].name == "OnLButtonDown");
    assert(handlers[3].kind == HandlerKind::PageEvent);
    assert(handlers[3].parameters.size() == 2);
    assert(handlers[3].parameters[1].name == "y");

    // Body span sanity: the recovered '{'..'}' for handler 0 encloses its calls.
    const std::string body = source.substr(
        handlers[0].bodyOffset, handlers[0].bodyEndOffset - handlers[0].bodyOffset);
    assert(body.front() == '{' && body.back() == '}');
    assert(body.find("Sound_Holder.PlayDSound(0)") != std::string::npos);
}

void testNoFalsePositives() {
    // Global-variable declarations and bare calls must not look like functions.
    const auto handlers = discoverHandlers("int global1=0;\nCString parentWnd;\nfoo();\n");
    assert(handlers.empty());

    // A call inside a body is not a top-level definition.
    const auto one = discoverHandlers("void OnTimer(){ SetTimer(1,100); if(x){y();} }");
    assert(one.size() == 1);
    assert(one[0].name == "OnTimer");
    assert(one[0].method == "SetTimer" || one[0].name == "OnTimer");
}

void testCollectCalls() {
    const std::string source = kIntroScript;
    const auto calls = collectCalls(source);

    int loadPage = 0, play = 0, playDSound = 0, badControl = 0;
    for (const auto& call : calls) {
        if (call.isBuiltin && call.name == "LoadPage") ++loadPage;
        if (call.isBuiltin && (call.name == "if" || call.name == "return")) ++badControl;
        if (!call.isBuiltin && call.component == "Transparent_Video_Holder" && call.name == "Play") ++play;
        if (!call.isBuiltin && call.component == "Sound_Holder" && call.name == "PlayDSound") ++playDSound;
    }
    assert(loadPage == 2);          // OnLButtonDown + inside TheEnd
    assert(play == 2);              // OnOpenPage + recursive tail in TheEnd
    assert(playDSound == 1);
    assert(badControl == 0);        // control keywords are not calls
}

} // namespace

int main() {
    testLexerBasics();
    testLexerCommentsAndStrings();
    testDiscoverIntroHandlers();
    testNoFalsePositives();
    testCollectCalls();
    return 0;
}
