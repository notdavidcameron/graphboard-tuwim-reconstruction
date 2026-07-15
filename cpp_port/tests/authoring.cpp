#include "graphboard/authoring.hpp"
#include "graphboard/binary_reader.hpp"
#include "graphboard/binary_writer.hpp"
#include "graphboard/component.hpp"
#include "graphboard/format.hpp"
#include "graphboard/holders.hpp"
#include "graphboard/runtime/project.hpp"
#include "graphboard/text.hpp"

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <filesystem>
#include <stdexcept>
#include <string>
#include <vector>

namespace {

using namespace graphboard;
using namespace graphboard::authoring;

void put32(std::vector<std::uint8_t>& b, std::size_t p, std::uint32_t v) {
    b[p]=static_cast<std::uint8_t>(v);b[p+1]=static_cast<std::uint8_t>(v>>8);
    b[p+2]=static_cast<std::uint8_t>(v>>16);b[p+3]=static_cast<std::uint8_t>(v>>24);
}

std::vector<std::uint8_t> tinyWav() {
    std::vector<std::uint8_t> b(48,0); std::copy_n("RIFF",4,b.begin()); put32(b,4,40);
    std::copy_n("WAVEfmt ",8,b.begin()+8); put32(b,16,16); b[20]=1;b[22]=1;
    put32(b,24,8000);put32(b,28,8000);b[32]=1;b[34]=8;std::copy_n("data",4,b.begin()+36);put32(b,40,4);
    b[44]=0x80;b[45]=0x80;b[46]=0x80;b[47]=0x80;return b;
}

IndexedImage image2() { return {2,2,{0,1,2,3},0}; }

ProjectDocument sampleProject() {
    ProjectDocument project; project.palette=makeRgb332Palette(); project.title="Synthetic";
    project.signature="Synthetic";project.startupPage="MAIN.BDF";project.globalScript="int navigationCount=0;\r\n";
    PageDocument page;page.id="main";page.fileName="MAIN.BDF";page.rect={0,0,8,8};page.usesProjectPalette=true;
    page.background=image2();page.backgroundColorIndex=0;
    page.script="void OnOpenPage(void)\r\n{ LoadGroup(\"CURSORS.GRP\"); SetCursor(1); }\r\nvoid OnClosePage(void)\r\n{}\r\n"
                "void HotSpot_Holder.LeftButtonClickOn(int rectID)\r\n{ Sound_Holder.PlayDSound(0); navigationCount=navigationCount+1; LoadPage(\"SECOND.BDF\"); }\r\n"
                "void Sound_Holder.EndPlaySound(int soundID)\r\n{ SetCursor(2); }\r\n";

    ComponentDocument hs;hs.wrapper=makeDefaultWrapper(HolderKind::HotSpotHolder);
    HotSpotComponent hstate;hstate.items.push_back({0,0,4,4,7,5,true,"hit"});hs.state=hstate;page.components.push_back(hs);
    ComponentDocument sp;sp.wrapper=makeDefaultWrapper(HolderKind::SpriteHolder);SpriteComponent sstate;
    SpriteDefinitionDocument def;def.name="sprite";def.frames.push_back({image2(),50,true});def.pixelHitTest=true;sstate.definitions.push_back(def);
    sstate.instances.push_back({0,1,1,1,0,true,true});sp.state=sstate;page.components.push_back(sp);
    ComponentDocument bm;bm.wrapper=makeDefaultWrapper(HolderKind::BitmapHolder);BitmapComponent bstate;
    bstate.items.push_back({"bitmap",2,2,0,false,true,image2()});bm.state=bstate;page.components.push_back(bm);
    ComponentDocument mb;mb.wrapper=makeDefaultWrapper(HolderKind::MultiBitmap);MultiBitmapComponent mstate;
    mstate.items.push_back({"multi",3,3,3,true,image2()});mb.state=mstate;page.components.push_back(mb);
    ComponentDocument snd;snd.wrapper=makeDefaultWrapper(HolderKind::SoundHolder);SoundComponent sound;
    sound.items.push_back({"beep",tinyWav()});snd.state=sound;page.components.push_back(snd);
    ComponentDocument vid;vid.wrapper=makeDefaultWrapper(HolderKind::VideoHolder);VideoComponent video;
    video.items.push_back({"clip.avi",4,4});vid.state=video;page.components.push_back(vid);
    project.pages.push_back(page);PageDocument second;second.id="second";second.fileName="SECOND.BDF";second.rect={0,0,8,8};
    second.script="void OnOpenPage(void)\r\n{}\r\nvoid OnClosePage(void)\r\n{}\r\n";project.pages.push_back(second);
    project.looseAssets.push_back({"clip.avi",{'R','I','F','F'}});

    graphboard::authoring::GroupDocument group;group.id="cursors";group.fileName="CURSORS.GRP";group.cursors.push_back({1,0,0,"arrow",image2()});
    project.groups.push_back(group);return project;
}

void testWriterStrings() {
    for (const std::size_t n : {std::size_t(0),std::size_t(254),std::size_t(255),
                                std::size_t(65534),std::size_t(65535)}) {
        const std::string input(n,'x');BinaryWriter writer;writer.writeArchiveString(input);
        BinaryReader reader(writer.bytes());assert(reader.readArchiveString()==input);assert(reader.eof());
    }
    BinaryWriter writer;const auto patch=writer.reserveU32();writer.writeU8(7);writer.patchU32(patch,0x12345678);
    BinaryReader reader(writer.bytes());assert(reader.readU32()==0x12345678);assert(reader.readU8()==7);
    bool threw=false;try{writer.patchU32(99,0);}catch(const std::out_of_range&){threw=true;}assert(threw);
    threw=false;try{BinaryWriter fixed;fixed.writeFixedString("toolong",3);}catch(const std::invalid_argument&){threw=true;}assert(threw);

    const auto guid=Guid::fromString("DA768116-5341-11D0-B444-008048EB5D40");
    BinaryWriter guidWriter;guidWriter.writeGuid(guid);
    assert(guidWriter.bytes().size()==guid.bytes.size());
    assert(std::equal(guidWriter.bytes().begin(),guidWriter.bytes().end(),guid.bytes.begin()));
    bool ok=false;const std::string polish=u8"Zażółć gęślą jaźń";
    const auto encoded=utf8ToCp1250(polish,&ok);assert(ok&&cp1250ToUtf8(encoded)==polish);
    assert(cp1250CaseFoldKey(u8"Ą.BDF")==cp1250CaseFoldKey(u8"ą.bdf"));
    (void)utf8ToCp1250(u8"emoji 🙂",&ok);assert(!ok);
    const std::string overlong{"\xc0\x80",2};(void)utf8ToCp1250(overlong,&ok);assert(!ok);

    ProjectDocument document;document.title="before";CommandStack commands;commands.record(document);document.title="after";
    assert(commands.undo(document)&&document.title=="before");assert(commands.redo(document)&&document.title=="after");
    const auto id=newDocumentId();assert(Guid::fromString(id).toString()==id);
    bool escaped=false;try{AssetStore(std::filesystem::temp_directory_path()).get("../outside");}catch(const std::invalid_argument&){escaped=true;}assert(escaped);
}

void testQuantization() {
    RgbaImage image;image.width=2;image.height=1;
    image.pixels={255,0,0,255, 0,255,0,0};
    const auto palette=generatePalette({image},7,128);
    const auto first=quantizeImage(image,palette,7,128);
    const auto second=quantizeImage(image,palette,7,128);
    assert(first.pixels==second.pixels&&first.pixels[1]==7);
    assert(first.sourceRgba==image.pixels);
    const auto converted=convertAsset(image,palette,7,128);assert(converted.diagnostic.opaquePixels==1&&converted.diagnostic.transparentPixels==1);
    assert(converted.bottomUpStride4Pixels.size()==4&&converted.indexed.pixels==first.pixels);
}

void testPageRoundTrip() {
    const auto project=sampleProject();ExportContext context;context.projectPalette=project.palette;context.bannerDate="Monday, January 01, 2001";
    const auto bytes=serializePage(project.pages[0],context);BinaryReader reader(bytes);const auto header=parseBdfHeader(reader);
    assert(header.pageRect.right==8&&header.paletteByteCount==1024&&header.dibByteCount>0&&header.minLayer==0&&header.maxLayer==5);
    const auto list=parseComponentListHeader(reader);assert(list.count==6);
    for(std::uint32_t i=0;i<list.count;++i){const auto item=parseComponentListItem(reader);const auto* info=lookupHolder(item.clsid);assert(info);
        switch(info->kind){case HolderKind::HotSpotHolder:{auto s=parseHotSpotHolderState(reader);assert(s.hotspots[0].id==7);break;}
        case HolderKind::SpriteHolder:{auto s=parseSpriteHolderState(reader);assert(s.definitions[0].frames.size()==1);assert(s.instances[0].visible==1);break;}
        case HolderKind::BitmapHolder:{auto s=parseBitmapHolderState(reader);assert(s.bitmaps[0].pixels==image2().pixels);break;}
        case HolderKind::MultiBitmap:{auto s=parseMultiBitmapState(reader);assert(s.records[0].width==2);break;}
        case HolderKind::SoundHolder:{auto s=parseSoundHolderState(reader);assert(s.sounds[0].looksLikeRiff);break;}
        case HolderKind::VideoHolder:{auto s=parseVideoHolderState(reader);assert(s.entries[0].name=="clip.avi");break;}
        default:assert(false);}
    }
    const auto script=parseScriptText(reader);assert(script.text.find("OnOpenPage")!=std::string::npos);
    const auto engine=parseScriptEngineState(reader);assert(engine.schemaVersion==4&&engine.parserState[3]!=0);
    assert(reader.readU32()==0&&reader.eof());
}

void testProjectAndGroup() {
    const auto project=sampleProject();const auto manifestBytes=serializeProjectManifest(project);BinaryReader mr(manifestBytes);
    const auto manifest=parseProjectManifest(mr);assert(manifest.startupPage=="MAIN.BDF"&&manifest.pageNames.size()==2);
    auto version0=sampleProject();version0.manifestVersion=0;version0.signature="not serialized";
    version0.globalScript="CString mHistory; int doTanca=0;";
    const auto version0Bytes=serializeProjectManifest(version0);BinaryReader version0Reader(version0Bytes);
    const auto version0Manifest=parseProjectManifest(version0Reader);
    assert(version0Manifest.version==0&&version0Manifest.encodedSignature.empty()&&
           version0Manifest.decodedSignature.empty()&&
           version0Manifest.globalScript==version0.globalScript&&version0Reader.eof());
    assert(serializeProjectManifest(version0)==version0Bytes);
    ExportContext context;context.projectPalette=project.palette;const auto groupBytes=serializeGroup(project.groups[0],context);BinaryReader gr(groupBytes);
    const auto group=parseGroupDocument(gr);assert(group.cursors.size()==1&&group.cursors[0].name=="arrow");
    const auto list=parseComponentListHeader(gr);assert(list.count==0&&gr.eof());
    std::string manyCalls="void OnOpenPage(void){";for(int i=0;i<120;++i)manyCalls+="SetCursor("+std::to_string(i)+");";manyCalls+="switch(1){case 1:switch(2){case 2:break;}break;default:break;}}void OnClosePage(void){}";
    const auto metadata=compileScriptMetadata(manyCalls);assert(metadata.builtinTokenOffsets.size()==100&&metadata.switchBlocks.size()==2);
    const std::string polishScript=u8"// żółć\r\nvoid OnOpenPage(void){}\r\nvoid OnClosePage(void){}";bool encodedOk=false;const auto polishBytes=utf8ToCp1250(polishScript,&encodedOk);const auto polishMetadata=compileScriptMetadata(polishScript);assert(encodedOk&&polishMetadata.parserState[3]==polishBytes.find('(',polishBytes.find("OnOpenPage")));
}

void testTransparentVideoRelocation() {
    constexpr std::size_t streamBytes=0x4e8,entryBytes=0x568;std::vector<std::uint8_t> privateState(8+4+streamBytes+entryBytes+4,0);put32(privateState,0,1);put32(privateState,4,1);put32(privateState,8,static_cast<std::uint32_t>(streamBytes));const auto streamStart=12u;put32(privateState,streamStart+0x80,1);put32(privateState,streamStart+0x84,1);const auto entry=12+streamBytes;put32(privateState,entry+0x80,1);put32(privateState,entry+0x84,1);put32(privateState,entry+0x558,0x11111111);put32(privateState,entry+0x55c,0x22222222);
    ProjectDocument project;project.palette=makeRgb332Palette();PageDocument page;page.id="tvh";page.fileName="TVH.BDF";ComponentDocument component;component.wrapper=makeDefaultWrapper(HolderKind::TransparentVideoHolder);component.state=OpaqueComponent{HolderKind::TransparentVideoHolder,privateState};component.editable=false;page.components.push_back(component);ExportContext context;context.projectPalette=project.palette;const auto bytes=serializePage(page,context);BinaryReader reader(bytes);parseBdfHeader(reader);parseComponentListHeader(reader);parseComponentListItem(reader);const auto privateOffset=reader.position();const auto storedEntry=privateOffset+entry;auto read32=[&](std::size_t p){return static_cast<std::uint32_t>(bytes[p])|(static_cast<std::uint32_t>(bytes[p+1])<<8)|(static_cast<std::uint32_t>(bytes[p+2])<<16)|(static_cast<std::uint32_t>(bytes[p+3])<<24);};assert(read32(storedEntry+0x558)==privateOffset+streamStart);assert(read32(storedEntry+0x55c)==privateOffset+streamStart+streamBytes);const auto parsed=parseTransparentVideoHolderState(reader);assert(parsed.entries.size()==1&&parsed.entries[0].stream.streamOffset==privateOffset+streamStart);
}

void testValidation() {
    auto project=sampleProject();auto diagnostics=validateProject(project);assert(std::none_of(diagnostics.begin(),diagnostics.end(),[](const Diagnostic& d){return d.severity==DiagnosticSeverity::Error;}));
    project.pages[0].script="void OnOpenPage(void){} void OnClosePage(void){} void HotSpot_Holder.LeftButtonClickOn(void){}";
    diagnostics=validateProject(project);assert(std::any_of(diagnostics.begin(),diagnostics.end(),[](const Diagnostic& d){return d.code=="script.callback_signature";}));
    project=sampleProject();project.pages[0].components[1].state=SpriteComponent{{SpriteDefinitionDocument{std::string(40,'x'),false,{SpriteFrameDocument{image2(),50,false}}}},{{0,0,0,0,0,true,false}}};
    diagnostics=validateProject(project);assert(std::any_of(diagnostics.begin(),diagnostics.end(),[](const Diagnostic& d){return d.code=="name.fixed";}));
    project=sampleProject();project.looseAssets.clear();diagnostics=validateProject(project);assert(std::any_of(diagnostics.begin(),diagnostics.end(),[](const Diagnostic& d){return d.code=="video.asset";}));
    project=sampleProject();PageDocument overridePage;overridePage.id="override";overridePage.fileName="OVERRIDE.BDF";overridePage.usesProjectPalette=false;overridePage.palette=project.palette;overridePage.palette[1].r^=0xff;overridePage.script="void OnOpenPage(void){LoadGroup(\"CURSORS.GRP\");} void OnClosePage(void){}";project.pages.push_back(overridePage);
    diagnostics=validateProject(project);assert(std::any_of(diagnostics.begin(),diagnostics.end(),[](const Diagnostic& d){return d.code=="group.palette"&&d.severity==DiagnosticSeverity::Warning;}));
}

void testProjectStorageAndExport() {
    const auto root=std::filesystem::temp_directory_path()/"graphboard_authoring_test";
    std::error_code ec;std::filesystem::remove_all(root,ec);auto project=sampleProject();RgbaImage retained;retained.width=2;retained.height=2;retained.pixels={255,0,0,255,0,255,0,20,0,0,255,255,255,255,255,255};project.pages[0].background=quantizeImage(retained,project.palette,3,77);
    saveProject(root/"project",project);assert(!std::filesystem::exists(root/"project"/"project.gbproj.tmp"));const auto loaded=loadProject(root/"project");assert(loaded.pages.size()==2&&loaded.groups.size()==1&&loaded.pages[0].id==project.pages[0].id);assert(loaded.pages[0].background&&loaded.pages[0].background->sourceRgba==retained.pixels&&loaded.pages[0].background->alphaCutoff==77);
    exportLegacyProject(root/"export",loaded);assert(std::filesystem::exists(root/"export"/"START.PRJ"));
    assert(std::filesystem::exists(root/"export"/"MAIN.BDF"));assert(std::filesystem::exists(root/"export"/"CURSORS.GRP"));
    auto version0Project=loaded;version0Project.manifestVersion=0;version0Project.signature="not present in version 0";version0Project.globalScript="CString mHistory; int doTanca=0;";
    exportLegacyProject(root/"export-v0",version0Project);const auto version0Source=BinaryReader::fromFile(root/"export-v0"/"START.PRJ").bytes();
    const auto importedVersion0=importLegacyProject(root/"export-v0"/"START.PRJ");assert(importedVersion0.manifestVersion==0&&importedVersion0.signature.empty());assert(serializeProjectManifest(importedVersion0)==version0Source);
    BinaryWriter dependencyWriter;dependencyWriter.writeBytes(serializePage(project.pages[1],ExportContext{project.palette,true,{}}));dependencyWriter.writeFileAtomic(root/"export"/"AAA_DEP.BDF");const auto importedManifest=importLegacyProject(root/"export"/"START.PRJ");assert(importedManifest.pages.size()==3&&importedManifest.pages[0].fileName=="MAIN.BDF"&&importedManifest.pages[1].fileName=="SECOND.BDF"&&importedManifest.pages[2].fileName=="AAA_DEP.BDF"&&!importedManifest.pages[2].listedInManifest);const auto sourceManifest=BinaryReader::fromFile(root/"export"/"START.PRJ").bytes();assert(serializeProjectManifest(importedManifest)==sourceManifest);saveProject(root/"manifest-project",importedManifest);assert(serializeProjectManifest(loadProject(root/"manifest-project"))==sourceManifest);auto changedManifest=importedManifest;changedManifest.signature="Changed";assert(serializeProjectManifest(changedManifest)!=sourceManifest);
    auto runtimeProject=graphboard::runtime::Project::loadFromFile(root/"export"/"START.PRJ");auto& page=runtimeProject->openStartupPage();
    assert(page.currentGroup()=="CURSORS.GRP"&&page.groupCursors().size()==1);page.lButtonDown(1,1);
    assert(page.pendingPage()=="SECOND.BDF");bool played=false;for(const auto& call:page.callLog())if(!call.builtin&&call.component=="Sound_Holder"&&call.name=="PlayDSound")played=true;assert(played);page.soundEnd(0);assert(page.cursor()==2);
    assert(runtimeProject->followPendingPage()&&runtimeProject->currentPageName()=="SECOND.BDF"&&runtimeProject->getGlobal("navigationCount").toInt()==1);
    const auto imported=importLegacyProject(root/"export"/"MAIN.BDF");ExportContext context;context.projectPalette=imported.palette;
    assert(serializePage(imported.pages[0],context)==imported.pages[0].sourceBytes);
    auto changedPalette=context;changedPalette.projectPalette[1].r^=0xff;assert(serializePage(imported.pages[0],changedPalette)!=imported.pages[0].sourceBytes);
    saveProject(root/"imported-project",imported);assert(std::filesystem::exists(root/"imported-project"/"assets")&&std::filesystem::exists(root/"imported-project"/"opaque"));
    auto unknownBytes=imported.pages[0].sourceBytes;BinaryReader unknownReader(unknownBytes);parseBdfHeader(unknownReader);parseComponentListHeader(unknownReader);const auto clsidOffset=unknownReader.position();assert(clsidOffset+16<=unknownBytes.size());std::fill_n(unknownBytes.begin()+static_cast<std::ptrdiff_t>(clsidOffset),16,0x5a);BinaryWriter unknownWriter;unknownWriter.writeBytes(unknownBytes);unknownWriter.writeFileAtomic(root/"UNKNOWN.BDF");const auto unknownProject=importLegacyProject(root/"UNKNOWN.BDF");assert(unknownProject.pages.size()==1&&unknownProject.pages[0].immutablePassthrough&&unknownProject.pages[0].components.empty());assert(serializePage(unknownProject.pages[0],context)==unknownBytes);
    auto duplicatePage=project.pages[0];duplicatePage.components.insert(duplicatePage.components.begin()+1,duplicatePage.components.front());const auto duplicateBytes=serializePage(duplicatePage,context);BinaryWriter duplicateWriter;duplicateWriter.writeBytes(duplicateBytes);duplicateWriter.writeFileAtomic(root/"DUPLICATE.BDF");const auto duplicateProject=importLegacyProject(root/"DUPLICATE.BDF");assert(duplicateProject.pages.size()==1&&duplicateProject.pages[0].components.size()==7);assert(!duplicateProject.pages[0].components[0].editable&&!duplicateProject.pages[0].components[1].editable);
    auto shifted=imported.pages[0];shifted.dirty=true;shifted.background=IndexedImage{4,4,std::vector<std::uint8_t>(16,1),0};shifted.script="// shifted offsets\r\n"+shifted.script;
    const auto shiftedBytes=serializePage(shifted,context);BinaryReader shiftedReader(shiftedBytes);parseBdfHeader(shiftedReader);const auto shiftedList=parseComponentListHeader(shiftedReader);bool relocatedSound=false,callbackChecked=false;
    for(std::uint32_t i=0;i<shiftedList.count;++i){const auto item=parseComponentListItem(shiftedReader);const auto kind=lookupHolder(item.clsid)->kind;const auto privateStart=shiftedReader.position();
        if(kind==HolderKind::HotSpotHolder){const auto expected=shifted.script.find('(',shifted.script.find("HotSpot_Holder.LeftButtonClickOn"));for(const auto& fn:item.wrapper.functions)if(fn.displayName=="LeftButtonClickOn"){assert(fn.dispatchIdOrOffset==expected);callbackChecked=true;}parseHotSpotHolderState(shiftedReader);}
        else if(kind==HolderKind::SpriteHolder)parseSpriteHolderState(shiftedReader);else if(kind==HolderKind::BitmapHolder)parseBitmapHolderState(shiftedReader);else if(kind==HolderKind::MultiBitmap)parseMultiBitmapState(shiftedReader);
        else if(kind==HolderKind::SoundHolder){const auto sound=parseSoundHolderState(shiftedReader);assert(sound.sounds.size()==1&&sound.sounds[0].looksLikeRiff&&sound.sounds[0].soundOffset<shiftedBytes.size());relocatedSound=true;}
        else if(kind==HolderKind::VideoHolder)parseVideoHolderState(shiftedReader);else assert(false);const auto privateEnd=shiftedReader.position();if(kind!=HolderKind::SoundHolder){const std::vector<std::uint8_t> regenerated(shiftedBytes.begin()+static_cast<std::ptrdiff_t>(privateStart),shiftedBytes.begin()+static_cast<std::ptrdiff_t>(privateEnd));assert(regenerated==imported.pages[0].components[i].sourcePrivateState);}}
    assert(relocatedSound&&callbackChecked);
    std::filesystem::remove_all(root,ec);
}

} // namespace

int main() {
    testWriterStrings();testQuantization();testPageRoundTrip();testProjectAndGroup();testTransparentVideoRelocation();testValidation();testProjectStorageAndExport();return 0;
}
