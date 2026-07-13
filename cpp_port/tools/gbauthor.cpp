#include "graphboard/authoring.hpp"
#include "graphboard/binary_writer.hpp"

#include <algorithm>
#include <cstdint>
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

namespace fs = std::filesystem;
using namespace graphboard::authoring;

namespace {

void usage() {
    std::cerr << "usage:\n"
                 "  gbauthor import <START.PRJ|PAGE.BDF|GROUP.GRP> <project-dir|file.gbproj>\n"
                 "  gbauthor validate <project-dir|file.gbproj>\n"
                 "  gbauthor export <project-dir|file.gbproj> <output-dir>\n"
                 "  gbauthor export-import <START.PRJ|PAGE.BDF|GROUP.GRP> <output-dir>\n"
                 "  gbauthor roundtrip <PAGE.BDF> <output.BDF>\n"
                 "  gbauthor verify-import <START.PRJ|PAGE.BDF|GROUP.GRP>\n"
                 "  gbauthor validate-import <START.PRJ|PAGE.BDF|GROUP.GRP>\n"
                 "  gbauthor acceptance <output-dir>\n";
}

void put32(std::vector<std::uint8_t>& bytes,std::size_t offset,std::uint32_t value){bytes[offset]=static_cast<std::uint8_t>(value);bytes[offset+1]=static_cast<std::uint8_t>(value>>8);bytes[offset+2]=static_cast<std::uint8_t>(value>>16);bytes[offset+3]=static_cast<std::uint8_t>(value>>24);}
std::vector<std::uint8_t> acceptanceWav(){std::vector<std::uint8_t> bytes(844,0);std::copy_n("RIFF",4,bytes.begin());put32(bytes,4,836);std::copy_n("WAVEfmt ",8,bytes.begin()+8);put32(bytes,16,16);bytes[20]=1;bytes[22]=1;put32(bytes,24,8000);put32(bytes,28,8000);bytes[32]=1;bytes[34]=8;std::copy_n("data",4,bytes.begin()+36);put32(bytes,40,800);std::fill(bytes.begin()+44,bytes.end(),0x80);return bytes;}
RgbaImage acceptanceArtwork(std::uint32_t width,std::uint32_t height,std::uint8_t phase=0){RgbaImage image;image.width=width;image.height=height;image.pixels.resize(static_cast<std::size_t>(width)*height*4);for(std::uint32_t y=0;y<height;++y)for(std::uint32_t x=0;x<width;++x){const auto p=(static_cast<std::size_t>(y)*width+x)*4;image.pixels[p]=static_cast<std::uint8_t>((x*4+phase*40)&255);image.pixels[p+1]=static_cast<std::uint8_t>((y*5+phase*20)&255);image.pixels[p+2]=static_cast<std::uint8_t>((x+y+phase*60)&255);image.pixels[p+3]=255;}return image;}

ProjectDocument acceptanceProject(){ProjectDocument project;project.title="GraphBoard Studio Acceptance";project.signature="GraphBoard Studio";project.palette=makeRgb332Palette();project.startupPage="MAIN.BDF";project.globalScript="int navigationCount=0;\r\n";
    PageDocument main;main.id=newDocumentId();main.fileName="MAIN.BDF";main.rect={0,0,64,48};main.background=quantizeImage(acceptanceArtwork(64,48),project.palette);main.script="void OnOpenPage(void){LoadGroup(\"CURSORS.GRP\");SetCursor(0);}\r\nvoid OnClosePage(void){}\r\nvoid HotSpot_Holder.LeftButtonClickOn(int rectID){Sound_Holder.PlayDSound(0);navigationCount=navigationCount+1;LoadPage(\"SECOND.BDF\");}\r\nvoid Sound_Holder.EndPlaySound(int soundID){SetCursor(2);}\r\n";
    ComponentDocument hotspot;hotspot.wrapper=makeDefaultWrapper(graphboard::HolderKind::HotSpotHolder);HotSpotComponent hotspotState;hotspotState.items.push_back({0,0,24,24,0,10,true,"next"});hotspot.state=hotspotState;main.components.push_back(hotspot);
    ComponentDocument sprite;sprite.wrapper=makeDefaultWrapper(graphboard::HolderKind::SpriteHolder);SpriteComponent spriteState;SpriteDefinitionDocument definition;definition.name="spinner";definition.pixelHitTest=true;definition.frames.push_back({quantizeImage(acceptanceArtwork(12,12,0),project.palette),80,true});definition.frames.push_back({quantizeImage(acceptanceArtwork(12,12,1),project.palette),80,true});spriteState.definitions.push_back(definition);spriteState.instances.push_back({0,30,8,2,0,true,true});sprite.state=spriteState;main.components.push_back(sprite);
    ComponentDocument bitmap;bitmap.wrapper=makeDefaultWrapper(graphboard::HolderKind::BitmapHolder);BitmapComponent bitmapState;bitmapState.items.push_back({"picture",8,28,1,false,true,quantizeImage(acceptanceArtwork(16,12,2),project.palette)});bitmap.state=bitmapState;main.components.push_back(bitmap);
    ComponentDocument multi;multi.wrapper=makeDefaultWrapper(graphboard::HolderKind::MultiBitmap);MultiBitmapComponent multiState;multiState.items.push_back({"overlay",42,28,3,true,quantizeImage(acceptanceArtwork(12,10,3),project.palette)});multi.state=multiState;main.components.push_back(multi);
    ComponentDocument sound;sound.wrapper=makeDefaultWrapper(graphboard::HolderKind::SoundHolder);SoundComponent soundState;soundState.items.push_back({"click",acceptanceWav()});sound.state=soundState;main.components.push_back(sound);
    ComponentDocument video;video.wrapper=makeDefaultWrapper(graphboard::HolderKind::VideoHolder);VideoComponent videoState;videoState.items.push_back({"sample.avi",20,16});video.state=videoState;main.components.push_back(video);project.pages.push_back(main);
    PageDocument second;second.id=newDocumentId();second.fileName="SECOND.BDF";second.rect={0,0,64,48};second.background=quantizeImage(acceptanceArtwork(64,48,4),project.palette);second.script="void OnOpenPage(void){}\r\nvoid OnClosePage(void){}\r\n";project.pages.push_back(second);
    GroupDocument group;group.id=newDocumentId();group.fileName="CURSORS.GRP";group.cursors.push_back({1,0,0,"arrow",quantizeImage(acceptanceArtwork(8,8,5),project.palette)});project.groups.push_back(group);
    project.looseAssets.push_back({"sample.avi",{'R','I','F','F',0,0,0,0,'A','V','I',' '}});return project;}

} // namespace

int run(int argc, const std::string& command, const fs::path& first,
        const fs::path& second) {
    if (argc < 3) { usage(); return 2; }
    try {
        if (command == "import" && argc == 4) {
            const auto project = importLegacyProject(first);
            saveProject(second, project);
            std::cout << "imported " << project.pages.size() << " pages and "
                      << project.groups.size() << " groups\n";
            return 0;
        }
        if (command == "validate" && argc == 3) {
            const auto project = loadProject(first);
            const auto diagnostics = validateProject(project);
            bool failed = false;
            for (const auto& d : diagnostics) {
                std::cout << (d.severity == DiagnosticSeverity::Error ? "error" : "warning")
                          << " [" << d.code << "] " << d.object << ": " << d.message << "\n";
                failed = failed || d.severity == DiagnosticSeverity::Error;
            }
            if (diagnostics.empty()) std::cout << "valid\n";
            return failed ? 1 : 0;
        }
        if (command == "export" && argc == 4) {
            exportLegacyProject(second, loadProject(first));
            std::cout << "export complete\n";
            return 0;
        }
        if(command=="export-import"&&argc==4){exportLegacyProject(second,importLegacyProject(first));std::cout<<"imported legacy files validated and exported\n";return 0;}
        if (command == "roundtrip" && argc == 4) {
            auto project = importLegacyProject(first);
            if (project.pages.size() != 1) throw std::runtime_error("roundtrip expects one BDF page");
            project.pages[0].dirty = true;
            ExportContext context; context.projectPalette = project.palette;
            context.preserveUnchanged = false;
            graphboard::BinaryWriter writer;
            writer.writeBytes(serializePage(project.pages[0], context));
            writer.writeFileAtomic(second);
            std::cout << "roundtrip complete\n";
            return 0;
        }
        if (command == "acceptance" && argc == 3) {
            const auto project=acceptanceProject();saveProject(first/"project",project);exportLegacyProject(first/"legacy",project);
            std::cout<<"acceptance project written to "<<(first/"project").string()<<" and "<<(first/"legacy").string()<<"\n";return 0;
        }
        if(command=="verify-import"&&argc==3){const auto project=importLegacyProject(first);ExportContext context;context.projectPalette=project.palette;context.preserveUnchanged=true;std::size_t checked=0;if(!project.sourceManifestBytes.empty()){if(serializeProjectManifest(project)!=project.sourceManifestBytes)throw std::runtime_error("unchanged START.PRJ was not preserved byte-for-byte");++checked;}for(const auto& page:project.pages){if(page.sourceBytes.empty()||serializePage(page,context)!=page.sourceBytes)throw std::runtime_error("unchanged page was not preserved: "+page.fileName);++checked;}for(const auto& group:project.groups){if(group.sourceBytes.empty()||serializeGroup(group,context)!=group.sourceBytes)throw std::runtime_error("unchanged group was not preserved: "+group.fileName);++checked;}std::cout<<"byte-identical preservation verified for "<<checked<<" imported files\n";return 0;}
        if(command=="validate-import"&&argc==3){const auto diagnostics=validateProject(importLegacyProject(first));std::size_t errors=0,warnings=0;for(const auto& diagnostic:diagnostics){if(diagnostic.severity==DiagnosticSeverity::Error)++errors;else ++warnings;std::cerr<<(diagnostic.severity==DiagnosticSeverity::Error?"error":"warning")<<" ["<<diagnostic.code<<"] "<<diagnostic.object<<": "<<diagnostic.message<<"\n";}std::cout<<errors<<" errors, "<<warnings<<" warnings\n";return errors?1:0;}
    } catch (const std::exception& e) {
        std::cerr << "gbauthor: " << e.what() << "\n";
        return 1;
    }
    usage();
    return 2;
}

#ifdef _WIN32
int wmain(int argc, wchar_t** argv) {
    const std::wstring wideCommand = argc > 1 ? argv[1] : L"";
    std::string command;
    command.reserve(wideCommand.size());
    for (const wchar_t c : wideCommand) {
        if (c > 0x7f) { usage(); return 2; }
        command.push_back(static_cast<char>(c));
    }
    return run(argc, command,
               argc > 2 ? fs::path(argv[2]) : fs::path{},
               argc > 3 ? fs::path(argv[3]) : fs::path{});
}
#else
int main(int argc, char** argv) {
    return run(argc, argc > 1 ? argv[1] : "",
               argc > 2 ? fs::u8path(argv[2]) : fs::path{},
               argc > 3 ? fs::u8path(argv[3]) : fs::path{});
}
#endif
