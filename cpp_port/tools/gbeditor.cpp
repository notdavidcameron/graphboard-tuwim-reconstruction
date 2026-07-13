// GraphBoard Studio: native authoring front-end for the reconstructed format
// and runtime. The document/codec layer lives in graphboard_authoring; this
// file is intentionally UI-only so project workflows remain headless-testable.

#include "graphboard/authoring.hpp"
#include "graphboard/binary_reader.hpp"
#include "graphboard/binary_writer.hpp"
#include "graphboard/render.hpp"
#include "graphboard/runtime/page.hpp"
#include "graphboard/runtime/project.hpp"
#include "graphboard/runtime/script.hpp"
#include "graphboard/text.hpp"

#include <windows.h>
#include <commdlg.h>
#include <d3d11.h>
#include <shobjidl.h>
#include <wincodec.h>
#include <mmsystem.h>
#include <mfapi.h>
#include <mfidl.h>
#include <mfreadwrite.h>

#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"

#include <algorithm>
#include <chrono>
#include <cmath>
#include <cctype>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <memory>
#include <optional>
#include <set>
#include <string>
#include <utility>
#include <variant>
#include <vector>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND, UINT, WPARAM, LPARAM);

namespace {

namespace fs = std::filesystem;
using namespace graphboard;
using namespace graphboard::authoring;

ID3D11Device* gDevice=nullptr;
ID3D11DeviceContext* gContext=nullptr;
IDXGISwapChain* gSwapChain=nullptr;
ID3D11RenderTargetView* gRenderTarget=nullptr;

template<class T> struct ComPtr {
    T* p=nullptr; ~ComPtr(){if(p)p->Release();}
    T** put(){if(p){p->Release();p=nullptr;}return &p;}
    T* operator->()const{return p;} explicit operator bool()const{return p!=nullptr;}
};

std::wstring utf8ToWide(const std::string& s){if(s.empty())return {};const int n=MultiByteToWideChar(CP_UTF8,0,s.data(),static_cast<int>(s.size()),nullptr,0);
    std::wstring out(n,L'\0');MultiByteToWideChar(CP_UTF8,0,s.data(),static_cast<int>(s.size()),out.data(),n);return out;}
std::string wideToUtf8(const std::wstring& s){if(s.empty())return {};const int n=WideCharToMultiByte(CP_UTF8,0,s.data(),static_cast<int>(s.size()),nullptr,0,nullptr,nullptr);
    std::string out(n,'\0');WideCharToMultiByte(CP_UTF8,0,s.data(),static_cast<int>(s.size()),out.data(),n,nullptr,nullptr);return out;}

std::vector<std::uint8_t> readFile(const fs::path& path){std::ifstream stream(path,std::ios::binary);if(!stream)throw std::runtime_error("Unable to open asset");
    stream.seekg(0,std::ios::end);const auto size=stream.tellg();stream.seekg(0,std::ios::beg);std::vector<std::uint8_t> bytes(static_cast<std::size_t>(size));
    if(!bytes.empty())stream.read(reinterpret_cast<char*>(bytes.data()),static_cast<std::streamsize>(bytes.size()));if(!stream)throw std::runtime_error("Unable to read asset");return bytes;}

std::optional<fs::path> fileDialog(bool save,const wchar_t* filter,const wchar_t* extension=L""){
    wchar_t path[MAX_PATH]={};OPENFILENAMEW ofn{};ofn.lStructSize=sizeof(ofn);ofn.hwndOwner=GetActiveWindow();ofn.lpstrFile=path;ofn.nMaxFile=MAX_PATH;
    ofn.lpstrFilter=filter;ofn.lpstrDefExt=extension;ofn.Flags=OFN_EXPLORER|OFN_PATHMUSTEXIST|(save?OFN_OVERWRITEPROMPT:OFN_FILEMUSTEXIST);
    const BOOL ok=save?GetSaveFileNameW(&ofn):GetOpenFileNameW(&ofn);if(!ok)return std::nullopt;return fs::path(path);
}

std::optional<fs::path> folderDialog(){ComPtr<IFileOpenDialog> dialog;if(FAILED(CoCreateInstance(CLSID_FileOpenDialog,nullptr,CLSCTX_INPROC_SERVER,IID_PPV_ARGS(dialog.put()))))return std::nullopt;
    DWORD options=0;dialog->GetOptions(&options);dialog->SetOptions(options|FOS_PICKFOLDERS|FOS_FORCEFILESYSTEM);if(FAILED(dialog->Show(GetActiveWindow())))return std::nullopt;
    ComPtr<IShellItem> item;if(FAILED(dialog->GetResult(item.put())))return std::nullopt;PWSTR value=nullptr;if(FAILED(item->GetDisplayName(SIGDN_FILESYSPATH,&value)))return std::nullopt;
    fs::path result(value);CoTaskMemFree(value);return result;}

std::vector<RgbaImage> loadWicFrames(const fs::path& path){ComPtr<IWICImagingFactory> factory;if(FAILED(CoCreateInstance(CLSID_WICImagingFactory,nullptr,CLSCTX_INPROC_SERVER,IID_PPV_ARGS(factory.put()))))throw std::runtime_error("WIC is unavailable");
    ComPtr<IWICBitmapDecoder> decoder;if(FAILED(factory->CreateDecoderFromFilename(path.c_str(),nullptr,GENERIC_READ,WICDecodeMetadataCacheOnLoad,decoder.put())))throw std::runtime_error("Unable to decode image");
    UINT count=0;decoder->GetFrameCount(&count);std::vector<RgbaImage> frames;frames.reserve(count);
    for(UINT i=0;i<count;++i){ComPtr<IWICBitmapFrameDecode> frame;if(FAILED(decoder->GetFrame(i,frame.put())))continue;UINT w=0,h=0;frame->GetSize(&w,&h);
        ComPtr<IWICFormatConverter> converter;if(FAILED(factory->CreateFormatConverter(converter.put())))continue;
        if(FAILED(converter->Initialize(frame.p,GUID_WICPixelFormat32bppRGBA,WICBitmapDitherTypeNone,nullptr,0,WICBitmapPaletteTypeCustom)))continue;
        RgbaImage image;image.width=w;image.height=h;image.pixels.resize(static_cast<std::size_t>(w)*h*4);
        if(SUCCEEDED(converter->CopyPixels(nullptr,w*4,static_cast<UINT>(image.pixels.size()),image.pixels.data())))frames.push_back(std::move(image));}
    if(frames.empty())throw std::runtime_error("Image contains no decodable frames");return frames;}

struct AviPreview {Image image;double durationSeconds=0;};
AviPreview decodeAviPreview(const fs::path& path){ComPtr<IMFAttributes> attrs;if(FAILED(MFCreateAttributes(attrs.put(),2)))throw std::runtime_error("Media Foundation attributes unavailable");
    attrs->SetUINT32(MF_SOURCE_READER_ENABLE_VIDEO_PROCESSING,TRUE);ComPtr<IMFSourceReader> reader;if(FAILED(MFCreateSourceReaderFromURL(path.c_str(),attrs.p,reader.put())))throw std::runtime_error("No Media Foundation source/codec can open this AVI");
    ComPtr<IMFMediaType> requested;if(FAILED(MFCreateMediaType(requested.put())))throw std::runtime_error("Unable to create AVI preview media type");requested->SetGUID(MF_MT_MAJOR_TYPE,MFMediaType_Video);requested->SetGUID(MF_MT_SUBTYPE,MFVideoFormat_RGB32);
    if(FAILED(reader->SetCurrentMediaType(MF_SOURCE_READER_FIRST_VIDEO_STREAM,nullptr,requested.p)))throw std::runtime_error("The system codec cannot convert this AVI to RGB32");
    ComPtr<IMFMediaType> current;if(FAILED(reader->GetCurrentMediaType(MF_SOURCE_READER_FIRST_VIDEO_STREAM,current.put())))throw std::runtime_error("Unable to query AVI dimensions");UINT32 width=0,height=0;if(FAILED(MFGetAttributeSize(current.p,MF_MT_FRAME_SIZE,&width,&height))||!width||!height)throw std::runtime_error("AVI has no video frame dimensions");
    UINT32 rawStride=width*4;current->GetUINT32(MF_MT_DEFAULT_STRIDE,&rawStride);const auto stride=static_cast<std::int32_t>(rawStride);const auto pitch=static_cast<std::size_t>(stride<0?-static_cast<std::int64_t>(stride):stride);
    ComPtr<IMFSample> sample;for(int attempt=0;attempt<120&&!sample;++attempt){DWORD stream=0,flags=0;LONGLONG timestamp=0;IMFSample* value=nullptr;const auto hr=reader->ReadSample(MF_SOURCE_READER_FIRST_VIDEO_STREAM,0,&stream,&flags,&timestamp,&value);if(FAILED(hr))throw std::runtime_error("AVI codec failed while decoding the first frame");sample.p=value;if(flags&MF_SOURCE_READERF_ENDOFSTREAM)break;}
    if(!sample)throw std::runtime_error("AVI contains no decodable video frame");ComPtr<IMFMediaBuffer> buffer;if(FAILED(sample->ConvertToContiguousBuffer(buffer.put())))throw std::runtime_error("Unable to access decoded AVI frame");BYTE* data=nullptr;DWORD maxLength=0,length=0;if(FAILED(buffer->Lock(&data,&maxLength,&length)))throw std::runtime_error("Unable to lock decoded AVI frame");
    AviPreview result;result.image.width=static_cast<int>(width);result.image.height=static_cast<int>(height);result.image.rgb.resize(static_cast<std::size_t>(width)*height*3);if(pitch*height>length){buffer->Unlock();throw std::runtime_error("Decoded AVI frame has an unexpected stride");}
    for(UINT32 y=0;y<height;++y){const auto sourceY=stride>=0?height-1-y:y;const auto* row=data+static_cast<std::size_t>(sourceY)*pitch;for(UINT32 x=0;x<width;++x){const auto dst=(static_cast<std::size_t>(y)*width+x)*3;result.image.rgb[dst]=row[x*4+2];result.image.rgb[dst+1]=row[x*4+1];result.image.rgb[dst+2]=row[x*4];}}buffer->Unlock();
    PROPVARIANT duration;PropVariantInit(&duration);if(SUCCEEDED(reader->GetPresentationAttribute(MF_SOURCE_READER_MEDIASOURCE,MF_PD_DURATION,&duration))&&duration.vt==VT_UI8)result.durationSeconds=duration.uhVal.QuadPart/10000000.0;PropVariantClear(&duration);return result;}

int stringResizeCallback(ImGuiInputTextCallbackData* data){if(data->EventFlag==ImGuiInputTextFlags_CallbackResize){auto* value=static_cast<std::string*>(data->UserData);
    value->resize(static_cast<std::size_t>(data->BufTextLen));data->Buf=value->data();}return 0;}
bool inputText(const char* label,std::string& value,ImGuiInputTextFlags flags=0){flags|=ImGuiInputTextFlags_CallbackResize;if(value.capacity()<256)value.reserve(256);
    return ImGui::InputText(label,value.data(),value.capacity()+1,flags,stringResizeCallback,&value);}
bool inputTextMultiline(const char* label,std::string& value,const ImVec2& size){if(value.capacity()<4096)value.reserve(4096);
    return ImGui::InputTextMultiline(label,value.data(),value.capacity()+1,size,ImGuiInputTextFlags_CallbackResize|ImGuiInputTextFlags_AllowTabInput,stringResizeCallback,&value);}

void createRenderTarget(){ID3D11Texture2D* back=nullptr;gSwapChain->GetBuffer(0,IID_PPV_ARGS(&back));if(back){gDevice->CreateRenderTargetView(back,nullptr,&gRenderTarget);back->Release();}}
void cleanupRenderTarget(){if(gRenderTarget){gRenderTarget->Release();gRenderTarget=nullptr;}}
bool createDevice(HWND hwnd){DXGI_SWAP_CHAIN_DESC sd{};sd.BufferCount=2;sd.BufferDesc.Format=DXGI_FORMAT_R8G8B8A8_UNORM;sd.BufferUsage=DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow=hwnd;sd.SampleDesc.Count=1;sd.Windowed=TRUE;sd.SwapEffect=DXGI_SWAP_EFFECT_DISCARD;UINT flags=0;
    const D3D_FEATURE_LEVEL levels[]={D3D_FEATURE_LEVEL_11_0,D3D_FEATURE_LEVEL_10_0};D3D_FEATURE_LEVEL obtained{};
    if(FAILED(D3D11CreateDeviceAndSwapChain(nullptr,D3D_DRIVER_TYPE_HARDWARE,nullptr,flags,levels,2,D3D11_SDK_VERSION,&sd,&gSwapChain,&gDevice,&obtained,&gContext)))return false;
    createRenderTarget();return true;}
void cleanupDevice(){cleanupRenderTarget();if(gSwapChain)gSwapChain->Release();if(gContext)gContext->Release();if(gDevice)gDevice->Release();gSwapChain=nullptr;gContext=nullptr;gDevice=nullptr;}

struct PreviewTexture {
    ID3D11ShaderResourceView* view=nullptr;int width=0,height=0;
    ~PreviewTexture(){reset();}void reset(){if(view)view->Release();view=nullptr;width=height=0;}
    void update(const Image& image){reset();if(image.width<=0||image.height<=0)return;std::vector<std::uint8_t> rgba(static_cast<std::size_t>(image.width)*image.height*4);
        for(std::size_t i=0,p=0;i<image.rgb.size();i+=3,p+=4){rgba[p]=image.rgb[i];rgba[p+1]=image.rgb[i+1];rgba[p+2]=image.rgb[i+2];rgba[p+3]=255;}
        D3D11_TEXTURE2D_DESC desc{};desc.Width=image.width;desc.Height=image.height;desc.MipLevels=1;desc.ArraySize=1;desc.Format=DXGI_FORMAT_R8G8B8A8_UNORM;desc.SampleDesc.Count=1;
        desc.Usage=D3D11_USAGE_DEFAULT;desc.BindFlags=D3D11_BIND_SHADER_RESOURCE;D3D11_SUBRESOURCE_DATA data{};data.pSysMem=rgba.data();data.SysMemPitch=image.width*4;
        ID3D11Texture2D* texture=nullptr;if(SUCCEEDED(gDevice->CreateTexture2D(&desc,&data,&texture))){gDevice->CreateShaderResourceView(texture,nullptr,&view);texture->Release();width=image.width;height=image.height;}}
};

struct Selection {int component=-1;int item=-1;};
struct ItemRect {int component=-1,item=-1,layer=0,x=0,y=0,w=0,h=0;};
bool sameSelection(const Selection& a,const Selection& b){return a.component==b.component&&a.item==b.item;}
enum class CanvasTool { Select, HotSpot };

struct App {
    ProjectDocument project;fs::path projectFile;int pageIndex=0,groupIndex=-1,paletteIndex=0,spriteFrameIndex=0,importTransparentIndex=0,alphaCutoff=128;Selection selection;std::vector<Selection> multiSelection;
    bool projectDirty=false,previewDirty=true,play=false,showGrid=true,snapToGrid=true,editGlobalScript=false;int gridSize=8;std::string status;
    CommandStack commands;PreviewTexture texture,videoTexture;std::string videoPreviewStatus;std::vector<std::uint8_t> previewBytes;BdfHeader previewHeader;
    std::unique_ptr<runtime::Page> runtimePage;std::unique_ptr<runtime::Project> runtimeProject;fs::path playDirectory;float zoom=1.0f;ImVec2 pan{0,0};CanvasTool canvasTool=CanvasTool::Select;
    bool dragging=false,resizing=false,boxSelecting=false,drawingHotSpot=false,transformChanged=false;ImVec2 dragStart{},gestureStart{},gestureCurrent{};ProjectDocument dragSnapshot;
    std::chrono::steady_clock::time_point lastAutosave=std::chrono::steady_clock::now();

    App(){newProject();}
    ~App(){cleanupPlayDirectory();}
    bool confirmDiscard()const{return !projectDirty||MessageBoxW(GetActiveWindow(),L"This project has unsaved changes. Discard them?",L"GraphBoard Studio",MB_ICONWARNING|MB_YESNO|MB_DEFBUTTON2)==IDYES;}
    fs::path autosaveDirectory()const{return projectFile.empty()?fs::temp_directory_path()/"GraphBoardStudio-Autosave":projectFile.parent_path()/".gbeditor-autosave";}
    bool hasAutosave()const{std::error_code ec;return fs::exists(autosaveDirectory()/"project.gbproj",ec)&&!ec;}
    void recoverAutosave(){try{project=loadProject(autosaveDirectory());pageIndex=project.pages.empty()?-1:0;groupIndex=pageIndex<0&&!project.groups.empty()?0:-1;selectOnly({});commands.clear();projectDirty=true;previewDirty=true;status="Autosave recovered";}catch(const std::exception& e){status=e.what();}}
    void openProject(const fs::path& path){auto loaded=loadProject(path);bool recovered=false;const auto autosaveDir=path.parent_path()/".gbeditor-autosave";const auto autosaveFile=autosaveDir/"project.gbproj";std::error_code ec;
        if(fs::exists(autosaveFile,ec)&&!ec){const auto autoTime=fs::last_write_time(autosaveFile,ec);ec.clear();const auto sourceTime=fs::last_write_time(path,ec);if(!ec&&autoTime>sourceTime&&MessageBoxW(GetActiveWindow(),L"A newer autosave is available. Recover it?",L"GraphBoard Studio",MB_ICONQUESTION|MB_YESNO)==IDYES){loaded=loadProject(autosaveDir);recovered=true;}}
        project=std::move(loaded);projectFile=path;pageIndex=project.pages.empty()?-1:0;groupIndex=pageIndex<0&&!project.groups.empty()?0:-1;selection={};multiSelection.clear();commands.clear();previewDirty=true;projectDirty=recovered;status=recovered?"Autosave recovered":"Project opened";}
    void saveProjectNow(){auto path=projectFile;if(path.empty()){auto selected=folderDialog();if(selected)path=*selected/"project.gbproj";}if(path.empty())return;try{saveProject(path,project);projectFile=path;projectDirty=false;status="Project saved";}catch(const std::exception& e){status=e.what();}}
    PageDocument* page(){return pageIndex>=0&&static_cast<std::size_t>(pageIndex)<project.pages.size()?&project.pages[pageIndex]:nullptr;}
    graphboard::authoring::GroupDocument* group(){return groupIndex>=0&&static_cast<std::size_t>(groupIndex)<project.groups.size()?&project.groups[groupIndex]:nullptr;}
    std::vector<ComponentDocument>* activeComponents(){if(auto* p=page())return &p->components;if(auto* g=group())return &g->components;return nullptr;}
    const Palette& palette()const{if(pageIndex>=0&&static_cast<std::size_t>(pageIndex)<project.pages.size()&&!project.pages[pageIndex].usesProjectPalette)return project.pages[pageIndex].palette;return project.palette;}
    void newProject(){project={};project.palette=makeRgb332Palette();PageDocument p;p.id=newDocumentId();p.fileName="PAGE1.BDF";p.palette=project.palette;project.pages.push_back(p);
        project.startupPage=p.fileName;pageIndex=0;groupIndex=-1;selection={};multiSelection.clear();projectFile.clear();commands.clear();projectDirty=true;previewDirty=true;status="New project";}
    void commit(ProjectDocument before){if(auto* p=page())p->dirty=true;
        commands.record(std::move(before));projectDirty=true;previewDirty=true;}
    void commitEditorState(ProjectDocument before){commands.record(std::move(before));projectDirty=true;}
    void commitStructure(ProjectDocument before){if(auto* p=page())p->dirty=true;if(auto* g=group())g->dirty=true;commands.record(std::move(before));projectDirty=true;previewDirty=true;}
    void undoOnce(){if(!commands.undo(project))return;pageIndex=std::min<int>(pageIndex,static_cast<int>(project.pages.size())-1);groupIndex=std::min<int>(groupIndex,static_cast<int>(project.groups.size())-1);previewDirty=true;projectDirty=true;}
    void redoOnce(){if(!commands.redo(project))return;pageIndex=std::min<int>(pageIndex,static_cast<int>(project.pages.size())-1);groupIndex=std::min<int>(groupIndex,static_cast<int>(project.groups.size())-1);previewDirty=true;projectDirty=true;}
    void navigateToObject(const std::string& object){for(std::size_t i=0;i<project.pages.size();++i)if(object==project.pages[i].fileName||object.rfind(project.pages[i].fileName+":",0)==0){pageIndex=static_cast<int>(i);groupIndex=-1;selectOnly({});const auto separator=object.find(':');if(separator!=std::string::npos){const auto holder=object.substr(separator+1);for(std::size_t c=0;c<project.pages[i].components.size();++c)if(holder.rfind(project.pages[i].components[c].wrapper.displayName,0)==0){selectOnly({static_cast<int>(c),-1});break;}}previewDirty=true;return;}for(std::size_t i=0;i<project.groups.size();++i)if(object==project.groups[i].fileName||object.rfind(project.groups[i].fileName+":",0)==0){pageIndex=-1;groupIndex=static_cast<int>(i);selectOnly({});return;}}

    runtime::Page* livePage(){return runtimeProject?runtimeProject->currentPage():runtimePage.get();}
    void cleanupPlayDirectory(){runtimeProject.reset();if(playDirectory.empty())return;std::error_code ec;const auto absolute=fs::absolute(playDirectory,ec);const auto temp=fs::absolute(fs::temp_directory_path(),ec);if(!ec&&absolute.parent_path()==temp&&absolute.filename().wstring().rfind(L"gbeditor-play-",0)==0)fs::remove_all(absolute,ec);playDirectory.clear();}
    void loadPreviewFile(const fs::path& path){BinaryReader file=BinaryReader::fromFile(path);previewBytes=file.bytes();BinaryReader headerReader(previewBytes);previewHeader=parseBdfHeader(headerReader);}
    void rebuildPreview(){previewDirty=false;runtimePage.reset();cleanupPlayDirectory();texture.reset();auto* p=page();if(!p)return;try{ExportContext context;context.projectPalette=project.palette;context.preserveUnchanged=false;
            if(play){playDirectory=fs::temp_directory_path()/("gbeditor-play-"+std::to_string(GetCurrentProcessId()));auto staged=project;staged.startupPage=p->fileName;exportLegacyProject(playDirectory,staged);runtimeProject=runtime::Project::loadFromFile(playDirectory/"START.PRJ");runtimeProject->openStartupPage();loadPreviewFile(playDirectory/p->fileName);}
            else{previewBytes=serializePage(*p,context);BinaryReader runtimeReader(previewBytes);runtimePage=runtime::Page::loadFromReader(runtimeReader,p->fileName);BinaryReader headerReader(previewBytes);previewHeader=parseBdfHeader(headerReader);}
            texture.update(renderPage(*livePage(),previewBytes,previewHeader));status=play?"Play runtime ready":"Preview ready";
        }catch(const std::exception& e){cleanupPlayDirectory();status=std::string("Preview: ")+e.what();}}
    bool followPreviewNavigation(){if(!runtimeProject)return false;try{if(!runtimeProject->followPendingPage())return false;const auto& name=runtimeProject->currentPageName();for(std::size_t i=0;i<project.pages.size();++i)if(cp1250CaseFoldKey(project.pages[i].fileName)==cp1250CaseFoldKey(name)){pageIndex=static_cast<int>(i);groupIndex=-1;loadPreviewFile(playDirectory/project.pages[i].fileName);break;}return true;}catch(const std::exception& e){status=std::string("Play navigation: ")+e.what();return false;}}
    void rerenderRuntime(){auto* current=livePage();if(!current)return;texture.update(renderPage(*current,previewBytes,previewHeader));}

    std::vector<ItemRect> itemRects()const{std::vector<ItemRect> out;if(pageIndex<0||static_cast<std::size_t>(pageIndex)>=project.pages.size())return out;const auto& p=project.pages[pageIndex];
        for(std::size_t ci=0;ci<p.components.size();++ci){if(!p.components[ci].editorVisible)continue;std::visit([&](const auto& state){using T=std::decay_t<decltype(state)>;
            if constexpr(std::is_same_v<T,HotSpotComponent>){for(std::size_t i=0;i<state.items.size();++i){const auto& x=state.items[i];out.push_back({static_cast<int>(ci),static_cast<int>(i),x.layer,x.left,x.top,x.right-x.left,x.bottom-x.top});}}
            else if constexpr(std::is_same_v<T,SpriteComponent>){for(std::size_t i=0;i<state.instances.size();++i){const auto& x=state.instances[i];int w=1,h=1;if(x.definitionIndex<state.definitions.size()&&!state.definitions[x.definitionIndex].frames.empty()){
                    const auto& frames=state.definitions[x.definitionIndex].frames;const auto phase=std::clamp(x.phase,0,static_cast<int>(frames.size())-1);w=frames[phase].image.width;h=frames[phase].image.height;}out.push_back({static_cast<int>(ci),static_cast<int>(i),x.layer,x.x,x.y,w,h});}}
            else if constexpr(std::is_same_v<T,BitmapComponent>){for(std::size_t i=0;i<state.items.size();++i){const auto& x=state.items[i];out.push_back({static_cast<int>(ci),static_cast<int>(i),x.layer,x.x,x.y,static_cast<int>(x.image.width),static_cast<int>(x.image.height)});}}
            else if constexpr(std::is_same_v<T,MultiBitmapComponent>){for(std::size_t i=0;i<state.items.size();++i){const auto& x=state.items[i];out.push_back({static_cast<int>(ci),static_cast<int>(i),x.layer,x.x,x.y,static_cast<int>(x.image.width),static_cast<int>(x.image.height)});}}
            else if constexpr(std::is_same_v<T,VideoComponent>){for(std::size_t i=0;i<state.items.size();++i){const auto& x=state.items[i];out.push_back({static_cast<int>(ci),static_cast<int>(i),0,x.x,x.y,std::max(x.previewWidth,1),std::max(x.previewHeight,1)});}}
        },p.components[ci].state);}return out;}

    bool isSelected(const Selection& value)const{return std::any_of(multiSelection.begin(),multiSelection.end(),[&](const Selection& selected){return sameSelection(selected,value);});}
    void selectOnly(Selection value){selection=value;multiSelection.clear();if(value.component>=0&&value.item>=0)multiSelection.push_back(value);}
    void toggleSelection(Selection value){const auto it=std::find_if(multiSelection.begin(),multiSelection.end(),[&](const Selection& selected){return sameSelection(selected,value);});if(it==multiSelection.end()){multiSelection.push_back(value);selection=value;}else{multiSelection.erase(it);selection=multiSelection.empty()?Selection{}:multiSelection.back();}}

    void offsetItem(Selection target,int dx,int dy){auto* p=page();if(!p||target.component<0||target.component>=static_cast<int>(p->components.size()))return;auto& component=p->components[target.component];if(component.editorLocked||!component.editable)return;
        std::visit([&](auto& state){using T=std::decay_t<decltype(state)>;const int i=target.item;
            if constexpr(std::is_same_v<T,HotSpotComponent>){if(i>=0&&i<static_cast<int>(state.items.size())){auto& v=state.items[i];v.left+=dx;v.right+=dx;v.top+=dy;v.bottom+=dy;}}
            else if constexpr(std::is_same_v<T,SpriteComponent>){if(i>=0&&i<static_cast<int>(state.instances.size())){state.instances[i].x+=dx;state.instances[i].y+=dy;}}
            else if constexpr(std::is_same_v<T,BitmapComponent>||std::is_same_v<T,MultiBitmapComponent>){if(i>=0&&i<static_cast<int>(state.items.size())){state.items[i].x+=dx;state.items[i].y+=dy;}}
            else if constexpr(std::is_same_v<T,VideoComponent>){if(i>=0&&i<static_cast<int>(state.items.size())){state.items[i].x+=dx;state.items[i].y+=dy;}}
        },component.state);component.privateDirty=true;p->dirty=true;}

    void moveSelection(int x,int y){auto* p=page();if(!p||selection.component<0||selection.component>=static_cast<int>(p->components.size()))return;auto& component=p->components[selection.component];if(component.editorLocked)return;if(snapToGrid&&gridSize>0){x=static_cast<int>(std::round(static_cast<double>(x)/gridSize))*gridSize;y=static_cast<int>(std::round(static_cast<double>(y)/gridSize))*gridSize;}
        const auto rects=itemRects();const auto found=std::find_if(rects.begin(),rects.end(),[&](const ItemRect& r){return r.component==selection.component&&r.item==selection.item;});if(found==rects.end())return;const int dx=x-found->x,dy=y-found->y;if(!dx&&!dy)return;
        const auto targets=multiSelection.empty()?std::vector<Selection>{selection}:multiSelection;for(const auto& target:targets)offsetItem(target,dx,dy);transformChanged=true;previewDirty=true;projectDirty=true;}

    void resizeSelectedHotSpot(int right,int bottom){auto* p=page();if(!p||selection.component<0||selection.component>=static_cast<int>(p->components.size()))return;auto& component=p->components[selection.component];if(component.editorLocked||!component.editable)return;auto* hotspots=std::get_if<HotSpotComponent>(&component.state);if(!hotspots||selection.item<0||selection.item>=static_cast<int>(hotspots->items.size()))return;auto& item=hotspots->items[selection.item];if(snapToGrid&&gridSize>0){right=static_cast<int>(std::round(static_cast<double>(right)/gridSize))*gridSize;bottom=static_cast<int>(std::round(static_cast<double>(bottom)/gridSize))*gridSize;}right=std::max(right,item.left+1);bottom=std::max(bottom,item.top+1);if(right==item.right&&bottom==item.bottom)return;item.right=right;item.bottom=bottom;component.privateDirty=true;p->dirty=true;previewDirty=true;projectDirty=true;transformChanged=true;}

    void createHotSpot(int x0,int y0,int x1,int y1){auto* p=page();if(!p||p->immutablePassthrough)return;if(snapToGrid&&gridSize>0){auto snap=[&](int v){return static_cast<int>(std::round(static_cast<double>(v)/gridSize))*gridSize;};x0=snap(x0);y0=snap(y0);x1=snap(x1);y1=snap(y1);}const int left=std::min(x0,x1),top=std::min(y0,y1),right=std::max(x0,x1),bottom=std::max(y0,y1);if(right<=left||bottom<=top){status="Hotspot drag was too small";return;}auto before=project;int componentIndex=-1;HotSpotComponent* hotspots=nullptr;for(std::size_t i=0;i<p->components.size();++i)if((hotspots=std::get_if<HotSpotComponent>(&p->components[i].state))){componentIndex=static_cast<int>(i);break;}if(!hotspots){ComponentDocument component;component.wrapper=makeDefaultWrapper(HolderKind::HotSpotHolder);component.state=HotSpotComponent{};p->components.push_back(std::move(component));componentIndex=static_cast<int>(p->components.size()-1);hotspots=std::get_if<HotSpotComponent>(&p->components.back().state);}int id=0,layer=0;for(const auto& item:hotspots->items){id=std::max(id,item.id+1);layer=std::max(layer,item.layer);}hotspots->items.push_back({left,top,right,bottom,id,layer,true,"hotspot"+std::to_string(id)});p->components[componentIndex].privateDirty=true;selectOnly({componentIndex,static_cast<int>(hotspots->items.size()-1)});commit(std::move(before));status="Hotspot created";}

    void reorderSelectedItem(int direction){auto* p=page();if(!p||selection.component<0||selection.component>=static_cast<int>(p->components.size())||selection.item<0)return;auto before=project;auto& component=p->components[selection.component];bool changed=false;int target=selection.item;
        std::visit([&](auto& state){using T=std::decay_t<decltype(state)>;if constexpr(std::is_same_v<T,HotSpotComponent>||std::is_same_v<T,BitmapComponent>||std::is_same_v<T,MultiBitmapComponent>||std::is_same_v<T,SoundComponent>||std::is_same_v<T,VideoComponent>){target=selection.item+direction;if(target>=0&&target<static_cast<int>(state.items.size())){std::swap(state.items[selection.item],state.items[target]);changed=true;}}else if constexpr(std::is_same_v<T,SpriteComponent>){target=selection.item+direction;if(target>=0&&target<static_cast<int>(state.instances.size())){std::swap(state.instances[selection.item],state.instances[target]);changed=true;}}},component.state);
        if(!changed)return;component.privateDirty=true;selection.item=target;multiSelection={selection};commitStructure(std::move(before));}

    void addHolder(HolderKind kind){auto* p=page();auto* g=group();if((p&&p->immutablePassthrough)||(g&&g->immutablePassthrough)){status="This imported board is immutable";return;}auto* components=p?&p->components:(g?&g->components:nullptr);if(!components)return;for(const auto& c:*components)if(lookupHolder(c.wrapper.clsid)&&lookupHolder(c.wrapper.clsid)->kind==kind){status="That holder already exists";return;}
        auto before=project;ComponentDocument c;c.wrapper=makeDefaultWrapper(kind);switch(kind){case HolderKind::HotSpotHolder:c.state=HotSpotComponent{};break;case HolderKind::SpriteHolder:c.state=SpriteComponent{};break;
            case HolderKind::BitmapHolder:c.state=BitmapComponent{};break;case HolderKind::MultiBitmap:c.state=MultiBitmapComponent{};break;case HolderKind::SoundHolder:c.state=SoundComponent{};break;case HolderKind::VideoHolder:c.state=VideoComponent{};break;default:return;}
        components->push_back(std::move(c));selectOnly({static_cast<int>(components->size()-1),-1});if(g)g->dirty=true;commit(std::move(before));}

    void requantizeProjectPaletteAssets(){auto convert=[&](IndexedImage& image,const Palette& palette){if(image.sourceRgba.empty())return false;RgbaImage source{image.width,image.height,image.sourceRgba};image=quantizeImage(source,palette,image.transparentIndex,image.alphaCutoff);return true;};
        for(auto& p:project.pages)if(p.usesProjectPalette){p.dirty=true;if(p.background)convert(*p.background,project.palette);for(auto& c:p.components){bool changed=false;std::visit([&](auto& state){using T=std::decay_t<decltype(state)>;
            if constexpr(std::is_same_v<T,SpriteComponent>)for(auto& d:state.definitions)for(auto& f:d.frames)changed|=convert(f.image,project.palette);
            else if constexpr(std::is_same_v<T,BitmapComponent>||std::is_same_v<T,MultiBitmapComponent>)for(auto& item:state.items)changed|=convert(item.image,project.palette);},c.state);c.privateDirty|=changed;}}
        for(auto& g:project.groups){for(auto& cursor:g.cursors)convert(cursor.image,project.palette);for(auto& c:g.components){bool changed=false;std::visit([&](auto& state){using T=std::decay_t<decltype(state)>;
            if constexpr(std::is_same_v<T,SpriteComponent>)for(auto& d:state.definitions)for(auto& f:d.frames)changed|=convert(f.image,project.palette);
            else if constexpr(std::is_same_v<T,BitmapComponent>||std::is_same_v<T,MultiBitmapComponent>)for(auto& item:state.items)changed|=convert(item.image,project.palette);},c.state);c.privateDirty|=changed;}}}

    void requantizePagePaletteAssets(){auto* p=page();if(!p||p->usesProjectPalette)return;auto convert=[&](IndexedImage& image){if(image.sourceRgba.empty())return false;RgbaImage source{image.width,image.height,image.sourceRgba};image=quantizeImage(source,p->palette,image.transparentIndex,image.alphaCutoff);return true;};if(p->background)convert(*p->background);for(auto& component:p->components){bool changed=false;std::visit([&](auto& state){using T=std::decay_t<decltype(state)>;if constexpr(std::is_same_v<T,SpriteComponent>)for(auto& definition:state.definitions)for(auto& frame:definition.frames)changed|=convert(frame.image);else if constexpr(std::is_same_v<T,BitmapComponent>||std::is_same_v<T,MultiBitmapComponent>)for(auto& item:state.items)changed|=convert(item.image);},component.state);component.privateDirty|=changed;}p->dirty=true;previewDirty=true;}

    void previewVideoAsset(const std::string& name){const auto same=[](std::string a,std::string b){std::transform(a.begin(),a.end(),a.begin(),[](unsigned char c){return static_cast<char>(std::tolower(c));});std::transform(b.begin(),b.end(),b.begin(),[](unsigned char c){return static_cast<char>(std::tolower(c));});return a==b;};const auto found=std::find_if(project.looseAssets.begin(),project.looseAssets.end(),[&](const LooseAsset& asset){return same(asset.fileName,name);});if(found==project.looseAssets.end()){videoPreviewStatus="AVI asset is not packaged in this project";videoTexture.reset();return;}const auto temp=fs::temp_directory_path()/("gbeditor-preview-"+std::to_string(GetCurrentProcessId())+".avi");try{BinaryWriter writer;writer.writeBytes(found->bytes);writer.writeFileAtomic(temp);const auto preview=decodeAviPreview(temp);videoTexture.update(preview.image);videoPreviewStatus="Media Foundation: "+std::to_string(preview.image.width)+"x"+std::to_string(preview.image.height)+", "+std::to_string(preview.durationSeconds)+" s";}catch(const std::exception& e){videoTexture.reset();videoPreviewStatus=std::string("AVI preview unavailable: ")+e.what();}std::error_code ec;fs::remove(temp,ec);}

    void importBackground(bool regeneratePalette){auto* p=page();if(!p||p->immutablePassthrough)return;auto path=fileDialog(false,L"Images\0*.png;*.bmp;*.jpg;*.jpeg;*.gif\0All files\0*.*\0");if(!path)return;try{const auto rgba=loadWicFrames(*path).front();auto before=project;
            if(regeneratePalette){project.palette=generatePalette({rgba},static_cast<std::uint8_t>(importTransparentIndex),static_cast<std::uint8_t>(alphaCutoff));requantizeProjectPaletteAssets();p=page();p->usesProjectPalette=true;}
            p->background=quantizeImage(rgba,palette(),static_cast<std::uint8_t>(importTransparentIndex),static_cast<std::uint8_t>(alphaCutoff));p->rect={0,0,static_cast<int>(rgba.width),static_cast<int>(rgba.height)};commit(std::move(before));status=regeneratePalette?"Background imported; shared palette regenerated":"Background imported";
        }catch(const std::exception& e){status=e.what();}}

    void importCursor(){auto* g=group();if(!g||g->immutablePassthrough)return;auto path=fileDialog(false,L"Cursor image\0*.png;*.bmp;*.jpg;*.jpeg;*.gif\0All files\0*.*\0");if(!path)return;try{auto before=project;const auto rgba=loadWicFrames(*path).front();
            CursorDocument cursor;cursor.name=wideToUtf8(path->stem().wstring());cursor.image=quantizeImage(rgba,project.palette,static_cast<std::uint8_t>(importTransparentIndex),static_cast<std::uint8_t>(alphaCutoff));g->cursors.push_back(std::move(cursor));g->dirty=true;commit(std::move(before));status="Cursor imported";
        }catch(const std::exception& e){status=e.what();}}

    void importLooseAsset(){auto path=fileDialog(false,L"All files\0*.*\0");if(!path)return;try{const auto name=wideToUtf8(path->filename().wstring());const auto same=[&](const LooseAsset& asset){return _stricmp(asset.fileName.c_str(),name.c_str())==0;};if(std::any_of(project.looseAssets.begin(),project.looseAssets.end(),same)){status="A loose asset with that filename already exists";return;}auto before=project;project.looseAssets.push_back({name,readFile(*path)});commitEditorState(std::move(before));status="Loose asset packaged";}catch(const std::exception& e){status=e.what();}}

    void importSprite(){auto* p=page();auto* g=group();auto* components=activeComponents();if(!components||(p&&p->immutablePassthrough)||(g&&g->immutablePassthrough))return;auto path=fileDialog(false,L"Images\0*.png;*.bmp;*.jpg;*.jpeg;*.gif\0All files\0*.*\0");if(!path)return;
        try{const auto rgba=loadWicFrames(*path);std::vector<SpriteFrameDocument> frames;for(const auto& image:rgba)frames.push_back({quantizeImage(image,palette(),static_cast<std::uint8_t>(importTransparentIndex),static_cast<std::uint8_t>(alphaCutoff)),50,true});
            auto before=project;SpriteComponent* state=nullptr;for(auto& c:*components)if((state=std::get_if<SpriteComponent>(&c.state)))break;if(!state){addHolder(HolderKind::SpriteHolder);components=activeComponents();state=std::get_if<SpriteComponent>(&components->back().state);before=project;}
            SpriteDefinitionDocument definition;definition.name=wideToUtf8(path->stem().wstring());definition.pixelHitTest=true;definition.frames=std::move(frames);state->definitions.push_back(std::move(definition));
            state->instances.push_back({static_cast<std::uint32_t>(state->definitions.size()-1),0,0,0,0,true,false});for(auto& c:*components)if(std::get_if<SpriteComponent>(&c.state)==state)c.privateDirty=true;if(g)g->dirty=true;commit(std::move(before));
        }catch(const std::exception& e){status=e.what();}}

    void importBitmap(bool multi){auto* p=page();auto* g=group();auto* components=activeComponents();if(!components||(p&&p->immutablePassthrough)||(g&&g->immutablePassthrough))return;auto path=fileDialog(false,L"Images\0*.png;*.bmp;*.jpg;*.jpeg\0All files\0*.*\0");if(!path)return;
        try{const auto image=quantizeImage(loadWicFrames(*path).front(),palette(),static_cast<std::uint8_t>(importTransparentIndex),static_cast<std::uint8_t>(alphaCutoff));auto before=project;if(multi){MultiBitmapComponent* state=nullptr;for(auto& c:*components)if((state=std::get_if<MultiBitmapComponent>(&c.state)))break;
                if(!state){addHolder(HolderKind::MultiBitmap);components=activeComponents();state=std::get_if<MultiBitmapComponent>(&components->back().state);before=project;}state->items.push_back({wideToUtf8(path->stem().wstring()),0,0,0,true,image});for(auto& c:*components)if(std::get_if<MultiBitmapComponent>(&c.state)==state)c.privateDirty=true;}
            else{BitmapComponent* state=nullptr;for(auto& c:*components)if((state=std::get_if<BitmapComponent>(&c.state)))break;
                if(!state){addHolder(HolderKind::BitmapHolder);components=activeComponents();state=std::get_if<BitmapComponent>(&components->back().state);before=project;}state->items.push_back({wideToUtf8(path->stem().wstring()),0,0,0,false,true,image});for(auto& c:*components)if(std::get_if<BitmapComponent>(&c.state)==state)c.privateDirty=true;}
            if(g)g->dirty=true;commit(std::move(before));}catch(const std::exception& e){status=e.what();}}

    void importSound(){auto* p=page();auto* g=group();auto* components=activeComponents();if(!components||(p&&p->immutablePassthrough)||(g&&g->immutablePassthrough))return;auto path=fileDialog(false,L"Wave audio\0*.wav\0All files\0*.*\0");if(!path)return;try{auto before=project;SoundComponent* state=nullptr;
            for(auto& c:*components)if((state=std::get_if<SoundComponent>(&c.state)))break;if(!state){addHolder(HolderKind::SoundHolder);components=activeComponents();state=std::get_if<SoundComponent>(&components->back().state);before=project;}
            state->items.push_back({wideToUtf8(path->stem().wstring()),readFile(*path)});for(auto& c:*components)if(std::get_if<SoundComponent>(&c.state)==state)c.privateDirty=true;if(g)g->dirty=true;commit(std::move(before));}catch(const std::exception& e){status=e.what();}}
    void importVideo(){auto* p=page();auto* g=group();auto* components=activeComponents();if(!components||(p&&p->immutablePassthrough)||(g&&g->immutablePassthrough))return;auto path=fileDialog(false,L"AVI video\0*.avi\0All files\0*.*\0");if(!path)return;try{auto before=project;VideoComponent* state=nullptr;
            for(auto& c:*components)if((state=std::get_if<VideoComponent>(&c.state)))break;if(!state){addHolder(HolderKind::VideoHolder);components=activeComponents();state=std::get_if<VideoComponent>(&components->back().state);before=project;}
            const auto name=wideToUtf8(path->filename().wstring());state->items.push_back({name,0,0});for(auto& c:*components)if(std::get_if<VideoComponent>(&c.state)==state)c.privateDirty=true;project.looseAssets.push_back({name,readFile(*path)});if(g)g->dirty=true;commit(std::move(before));
            try{const auto preview=decodeAviPreview(*path);state->items.back().previewWidth=preview.image.width;state->items.back().previewHeight=preview.image.height;videoTexture.update(preview.image);videoPreviewStatus="Media Foundation: "+std::to_string(preview.image.width)+"x"+std::to_string(preview.image.height)+", "+std::to_string(preview.durationSeconds)+" s";}catch(const std::exception& codec){videoTexture.reset();videoPreviewStatus=std::string("AVI imported; preview unavailable: ")+codec.what();}
        }catch(const std::exception& e){status=e.what();}}
};

void drawMenu(App& app){if(!ImGui::BeginMainMenuBar())return;if(ImGui::BeginMenu("File")){if(ImGui::MenuItem("New","Ctrl+N")&&app.confirmDiscard())app.newProject();
        if(ImGui::MenuItem("Open project...","Ctrl+O")&&app.confirmDiscard()){if(auto path=fileDialog(false,L"GraphBoard project\0*.gbproj\0All files\0*.*\0")){try{app.openProject(*path);}catch(const std::exception& e){app.status=e.what();}}}
        if(ImGui::MenuItem("Import legacy...")&&app.confirmDiscard()){if(auto path=fileDialog(false,L"GraphBoard files\0START.PRJ;*.bdf;*.grp\0All files\0*.*\0")){try{app.project=importLegacyProject(*path);app.projectFile.clear();app.pageIndex=app.project.pages.empty()?-1:0;app.groupIndex=app.pageIndex<0&&!app.project.groups.empty()?0:-1;app.previewDirty=true;app.projectDirty=true;app.status="Legacy project imported";}catch(const std::exception& e){app.status=e.what();}}}
        if(ImGui::MenuItem("Save project","Ctrl+S"))app.saveProjectNow();
        if(ImGui::MenuItem("Recover autosave",nullptr,false,app.hasAutosave())&&app.confirmDiscard())app.recoverAutosave();
        if(ImGui::MenuItem("Export binaries...")){if(auto folder=folderDialog())try{exportLegacyProject(*folder,app.project);app.status="Export complete";}catch(const std::exception& e){app.status=e.what();}}
        if(ImGui::MenuItem("Exit"))SendMessageW(GetActiveWindow(),WM_CLOSE,0,0);ImGui::EndMenu();}
    if(ImGui::BeginMenu("Edit")){if(ImGui::MenuItem("Undo","Ctrl+Z",false,app.commands.canUndo()))app.undoOnce();if(ImGui::MenuItem("Redo","Ctrl+Y",false,app.commands.canRedo()))app.redoOnce();ImGui::EndMenu();}
    ImGui::Separator();ImGui::TextUnformatted(app.status.c_str());ImGui::EndMainMenuBar();}

void drawProject(App& app){ImGui::Begin("Project");if(ImGui::Button("+ Page")){auto before=app.project;PageDocument page;page.id=newDocumentId();page.fileName="PAGE"+std::to_string(app.project.pages.size()+1)+".BDF";
        page.palette=app.project.palette;app.project.pages.push_back(page);app.pageIndex=static_cast<int>(app.project.pages.size()-1);app.groupIndex=-1;if(app.project.startupPage.empty())app.project.startupPage=page.fileName;app.commit(std::move(before));}
    ImGui::SameLine();if(ImGui::Button("+ Group")){auto before=app.project;graphboard::authoring::GroupDocument group;group.id=newDocumentId();group.fileName="GROUP"+std::to_string(app.project.groups.size()+1)+".GRP";app.project.groups.push_back(group);app.commit(std::move(before));}
    {auto before=app.project;bool changed=false;changed|=inputText("Project title",app.project.title);changed|=inputText("Signature",app.project.signature);int preset=static_cast<int>(app.project.audioPresetIndex);if(ImGui::InputInt("Audio preset",&preset)){app.project.audioPresetIndex=static_cast<std::uint32_t>(std::max(preset,0));changed=true;}if(changed)app.commitEditorState(std::move(before));}
    if(ImGui::TreeNodeEx("Pages",ImGuiTreeNodeFlags_DefaultOpen)){for(std::size_t i=0;i<app.project.pages.size();++i){const auto& p=app.project.pages[i];ImGuiTreeNodeFlags flags=ImGuiTreeNodeFlags_Leaf|ImGuiTreeNodeFlags_NoTreePushOnOpen;
            if(static_cast<int>(i)==app.pageIndex)flags|=ImGuiTreeNodeFlags_Selected;std::string label=(app.project.startupPage==p.fileName?"* ":"")+p.fileName;
            ImGui::TreeNodeEx(reinterpret_cast<void*>(i+1),flags,"%s%s%s",label.c_str(),p.listedInManifest?"":" [dependency]",p.immutablePassthrough?" [locked]":"");if(ImGui::IsItemClicked()){app.pageIndex=static_cast<int>(i);app.groupIndex=-1;app.selectOnly({});app.previewDirty=true;}}
        ImGui::TreePop();}
    if(ImGui::TreeNodeEx("Groups",ImGuiTreeNodeFlags_DefaultOpen)){for(std::size_t i=0;i<app.project.groups.size();++i){const auto& g=app.project.groups[i];ImGuiTreeNodeFlags flags=ImGuiTreeNodeFlags_Leaf|ImGuiTreeNodeFlags_NoTreePushOnOpen;
            if(static_cast<int>(i)==app.groupIndex)flags|=ImGuiTreeNodeFlags_Selected;ImGui::TreeNodeEx(reinterpret_cast<void*>(app.project.pages.size()+i+1),flags,"%s%s%s",g.fileName.c_str(),g.listedInManifest?"":" [dependency]",g.immutablePassthrough?" [locked]":"");
            if(ImGui::IsItemClicked()){app.pageIndex=-1;app.groupIndex=static_cast<int>(i);app.selectOnly({});app.previewDirty=true;}}ImGui::TreePop();}
    ImGui::Separator();if(auto* p=app.page()){ImGui::Text("Startup: %s",app.project.startupPage.c_str());if(ImGui::Button("Set selected as startup")){auto before=app.project;app.project.startupPage=p->fileName;app.commit(std::move(before));}}
    ImGui::End();}

void drawCanvas(App& app){
    ImGui::Begin("Scene");
    if(ImGui::Button(app.play?"Stop":"Play")){app.play=!app.play;app.previewDirty=true;}
    ImGui::SameLine();if(ImGui::RadioButton("Select",app.canvasTool==CanvasTool::Select))app.canvasTool=CanvasTool::Select;
    ImGui::SameLine();if(ImGui::RadioButton("Draw hotspot",app.canvasTool==CanvasTool::HotSpot))app.canvasTool=CanvasTool::HotSpot;
    ImGui::SameLine();ImGui::Checkbox("Grid",&app.showGrid);ImGui::SameLine();ImGui::Checkbox("Snap",&app.snapToGrid);
    ImGui::SameLine();ImGui::SetNextItemWidth(70);ImGui::InputInt("Grid size",&app.gridSize);app.gridSize=std::clamp(app.gridSize,1,256);
    ImGui::SameLine();ImGui::Text("Zoom %.0f%%",app.zoom*100);
    if(app.previewDirty)app.rebuildPreview();

    const ImVec2 avail=ImGui::GetContentRegionAvail();
    ImGui::InvisibleButton("canvas",avail,ImGuiButtonFlags_MouseButtonLeft|ImGuiButtonFlags_MouseButtonMiddle);
    const ImVec2 canvas=ImGui::GetItemRectMin();const ImVec2 canvasMax=ImGui::GetItemRectMax();const bool hovered=ImGui::IsItemHovered();const bool active=ImGui::IsItemActive();
    if(hovered&&ImGui::GetIO().MouseWheel!=0)app.zoom=std::clamp(app.zoom+ImGui::GetIO().MouseWheel*0.1f,0.1f,8.0f);
    if((hovered||active)&&ImGui::IsMouseDragging(ImGuiMouseButton_Middle)){app.pan.x+=ImGui::GetIO().MouseDelta.x;app.pan.y+=ImGui::GetIO().MouseDelta.y;}
    const ImVec2 origin{canvas.x+app.pan.x,canvas.y+app.pan.y};auto boardPoint=[&](ImVec2 mouse){return ImVec2{(mouse.x-origin.x)/app.zoom,(mouse.y-origin.y)/app.zoom};};
    auto* draw=ImGui::GetWindowDrawList();draw->PushClipRect(canvas,canvasMax,true);draw->AddRectFilled(canvas,canvasMax,IM_COL32(30,30,34,255));
    if(app.showGrid&&app.page()&&app.gridSize*app.zoom>=3){const auto* p=app.page();const int width=p->rect.right-p->rect.left,height=p->rect.bottom-p->rect.top;for(int x=0;x<=width;x+=app.gridSize)draw->AddLine({origin.x+x*app.zoom,origin.y},{origin.x+x*app.zoom,origin.y+height*app.zoom},IM_COL32(90,90,100,90));for(int y=0;y<=height;y+=app.gridSize)draw->AddLine({origin.x,origin.y+y*app.zoom},{origin.x+width*app.zoom,origin.y+y*app.zoom},IM_COL32(90,90,100,90));}
    if(app.texture.view)draw->AddImage(reinterpret_cast<ImTextureID>(app.texture.view),origin,{origin.x+app.texture.width*app.zoom,origin.y+app.texture.height*app.zoom});
    const auto rects=app.itemRects();std::optional<ItemRect> primaryRect;
    for(const auto& r:rects){const Selection value{r.component,r.item};const bool selected=app.isSelected(value);const bool primary=sameSelection(app.selection,value);const ImU32 color=selected?IM_COL32(255,220,0,255):IM_COL32(0,220,255,180);draw->AddRect({origin.x+r.x*app.zoom,origin.y+r.y*app.zoom},{origin.x+(r.x+r.w)*app.zoom,origin.y+(r.y+r.h)*app.zoom},color,0,0,primary?3.0f:(selected?2.0f:1.0f));if(primary)primaryRect=r;}
    bool primaryIsHotSpot=false;if(primaryRect&&app.page()&&app.selection.component>=0&&app.selection.component<static_cast<int>(app.page()->components.size()))primaryIsHotSpot=std::holds_alternative<HotSpotComponent>(app.page()->components[app.selection.component].state);
    if(primaryRect&&primaryIsHotSpot){const ImVec2 corner{origin.x+(primaryRect->x+primaryRect->w)*app.zoom,origin.y+(primaryRect->y+primaryRect->h)*app.zoom};draw->AddRectFilled({corner.x-5,corner.y-5},{corner.x+5,corner.y+5},IM_COL32(255,220,0,255));}
    if(app.boxSelecting||app.drawingHotSpot){const ImVec2 a{origin.x+app.gestureStart.x*app.zoom,origin.y+app.gestureStart.y*app.zoom};const ImVec2 b{origin.x+app.gestureCurrent.x*app.zoom,origin.y+app.gestureCurrent.y*app.zoom};draw->AddRectFilled({std::min(a.x,b.x),std::min(a.y,b.y)},{std::max(a.x,b.x),std::max(a.y,b.y)},app.drawingHotSpot?IM_COL32(255,90,80,40):IM_COL32(80,160,255,35));draw->AddRect({std::min(a.x,b.x),std::min(a.y,b.y)},{std::max(a.x,b.x),std::max(a.y,b.y)},app.drawingHotSpot?IM_COL32(255,90,80,255):IM_COL32(80,160,255,255));}
    draw->PopClipRect();

    const ImVec2 board=boardPoint(ImGui::GetIO().MousePos);const int bx=static_cast<int>(board.x),by=static_cast<int>(board.y);
    if(app.play&&app.livePage()&&hovered){auto* live=app.livePage();live->mouseMove(bx,by);if(ImGui::IsMouseClicked(ImGuiMouseButton_Left)){live->lButtonDown(bx,by);live->lButtonUp(bx,by);app.followPreviewNavigation();app.rerenderRuntime();}}
    else if(!app.play){
        if(hovered&&ImGui::IsMouseClicked(ImGuiMouseButton_Left)){
            app.transformChanged=false;app.gestureStart=board;app.gestureCurrent=board;
            if(app.canvasTool==CanvasTool::HotSpot){app.drawingHotSpot=true;}
            else{
                bool handle=false;if(primaryRect&&primaryIsHotSpot){const float px=origin.x+(primaryRect->x+primaryRect->w)*app.zoom,py=origin.y+(primaryRect->y+primaryRect->h)*app.zoom;handle=std::abs(ImGui::GetIO().MousePos.x-px)<=8&&std::abs(ImGui::GetIO().MousePos.y-py)<=8;}
                if(handle){app.resizing=true;app.dragSnapshot=app.project;}
                else{ItemRect best;bool found=false;for(const auto& r:rects)if(bx>=r.x&&by>=r.y&&bx<r.x+r.w&&by<r.y+r.h&&(!found||r.layer>best.layer||(r.layer==best.layer&&(r.component>best.component||(r.component==best.component&&r.item>best.item))))){best=r;found=true;}
                    if(found){const Selection hit{best.component,best.item};if(ImGui::GetIO().KeyCtrl)app.toggleSelection(hit);else if(!app.isSelected(hit))app.selectOnly(hit);if(app.isSelected(hit)){app.dragging=true;app.dragStart={static_cast<float>(bx-best.x),static_cast<float>(by-best.y)};app.dragSnapshot=app.project;}}
                    else{if(!ImGui::GetIO().KeyCtrl)app.selectOnly({});app.boxSelecting=true;}}
            }
        }
        if((hovered||active)&&ImGui::IsMouseDown(ImGuiMouseButton_Left)){app.gestureCurrent=board;if(app.dragging)app.moveSelection(bx-static_cast<int>(app.dragStart.x),by-static_cast<int>(app.dragStart.y));else if(app.resizing)app.resizeSelectedHotSpot(bx,by);}
        if(ImGui::IsMouseReleased(ImGuiMouseButton_Left)){
            if(app.dragging||app.resizing){if(app.transformChanged)app.commit(std::move(app.dragSnapshot));app.dragging=false;app.resizing=false;app.transformChanged=false;}
            if(app.boxSelecting){app.boxSelecting=false;const int left=static_cast<int>(std::min(app.gestureStart.x,app.gestureCurrent.x)),top=static_cast<int>(std::min(app.gestureStart.y,app.gestureCurrent.y)),right=static_cast<int>(std::max(app.gestureStart.x,app.gestureCurrent.x)),bottom=static_cast<int>(std::max(app.gestureStart.y,app.gestureCurrent.y));app.multiSelection.clear();for(const auto& r:rects)if(r.x<right&&r.x+r.w>left&&r.y<bottom&&r.y+r.h>top)app.multiSelection.push_back({r.component,r.item});app.selection=app.multiSelection.empty()?Selection{}:app.multiSelection.back();}
            if(app.drawingHotSpot){app.drawingHotSpot=false;app.createHotSpot(static_cast<int>(app.gestureStart.x),static_cast<int>(app.gestureStart.y),static_cast<int>(app.gestureCurrent.x),static_cast<int>(app.gestureCurrent.y));app.canvasTool=CanvasTool::Select;}
        }
    }
    ImGui::End();
}

void drawInspector(App& app){ImGui::Begin("Inspector");auto* p=app.page();if(!p){auto* g=app.group();if(!g){ImGui::TextUnformatted("No board selected");ImGui::End();return;}if(g->immutablePassthrough){ImGui::TextUnformatted("Immutable passthrough group (unknown component boundary)");ImGui::End();return;}auto editGroup=[&](auto fn){auto before=app.project;if(fn()){app.group()->dirty=true;app.commit(std::move(before));}};
        ImGui::TextUnformatted("Group");editGroup([&]{return inputText("Filename",g->fileName);});editGroup([&]{return ImGui::Checkbox("Listed in START.PRJ",&g->listedInManifest);});if(ImGui::Button("Import cursor image"))app.importCursor();
        for(std::size_t i=0;i<g->cursors.size();++i){auto& cursor=g->cursors[i];ImGui::PushID(static_cast<int>(i));if(ImGui::Selectable(cursor.name.c_str(),app.selection.item==static_cast<int>(i))){app.selection={-1,static_cast<int>(i)};app.multiSelection.clear();}
            if(app.selection.item==static_cast<int>(i)){editGroup([&]{return inputText("Name",cursor.name);});editGroup([&]{return ImGui::InputInt("Hot X",&cursor.hotX);});editGroup([&]{return ImGui::InputInt("Hot Y",&cursor.hotY);});int flags=static_cast<int>(cursor.flags);editGroup([&]{if(!ImGui::InputInt("Flags",&flags))return false;app.group()->cursors[i].flags=static_cast<std::uint32_t>(std::max(flags,0));return true;});}ImGui::PopID();}
        ImGui::SeparatorText("Group holders");if(ImGui::Button("HotSpot"))app.addHolder(HolderKind::HotSpotHolder);ImGui::SameLine();if(ImGui::Button("Sprite"))app.addHolder(HolderKind::SpriteHolder);ImGui::SameLine();if(ImGui::Button("Bitmap"))app.addHolder(HolderKind::BitmapHolder);ImGui::SameLine();if(ImGui::Button("MultiBitmap"))app.addHolder(HolderKind::MultiBitmap);ImGui::SameLine();if(ImGui::Button("Sound"))app.addHolder(HolderKind::SoundHolder);ImGui::SameLine();if(ImGui::Button("Video"))app.addHolder(HolderKind::VideoHolder);
        if(ImGui::Button("Import sprite/GIF"))app.importSprite();ImGui::SameLine();if(ImGui::Button("Import bitmap"))app.importBitmap(false);ImGui::SameLine();if(ImGui::Button("Import multibitmap"))app.importBitmap(true);if(ImGui::Button("Import WAV"))app.importSound();ImGui::SameLine();if(ImGui::Button("Import AVI"))app.importVideo();
        for(std::size_t i=0;i<g->components.size();++i){const auto& component=g->components[i];if(ImGui::Selectable((component.wrapper.displayName+(component.editable?"":" [preserved]")).c_str(),app.selection.component==static_cast<int>(i))){app.selection={static_cast<int>(i),-1};app.multiSelection.clear();}}
        if(app.selection.component>=0&&app.selection.component<static_cast<int>(g->components.size())){auto& component=g->components[app.selection.component];ImGui::SeparatorText(component.wrapper.displayName.c_str());if(!component.editable)ImGui::Text("Preserved private bytes: %zu",component.sourcePrivateState.size());else{auto editComponent=[&](auto fn){auto before=app.project;if(fn()){auto& changed=app.group()->components[app.selection.component];changed.privateDirty=true;app.group()->dirty=true;app.commitStructure(std::move(before));}};std::visit([&](auto& state){using T=std::decay_t<decltype(state)>;
                    if constexpr(std::is_same_v<T,HotSpotComponent>){if(ImGui::Button("Add group hotspot")){auto before=app.project;int id=0;for(const auto& item:state.items)id=std::max(id,item.id+1);state.items.push_back({0,0,32,32,id,0,true,"groupHotspot"+std::to_string(id)});component.privateDirty=true;g->dirty=true;app.commitStructure(std::move(before));}for(std::size_t i=0;i<state.items.size();++i){ImGui::PushID(static_cast<int>(i));auto& item=state.items[i];ImGui::Text("%d",item.id);ImGui::SameLine();editComponent([&]{return inputText("Name",item.name);});int bounds[4]={item.left,item.top,item.right,item.bottom};editComponent([&]{if(!ImGui::InputInt4("Bounds",bounds))return false;auto& current=std::get<HotSpotComponent>(app.group()->components[app.selection.component].state).items[i];current.left=bounds[0];current.top=bounds[1];current.right=bounds[2];current.bottom=bounds[3];return true;});ImGui::PopID();}}
                    else if constexpr(std::is_same_v<T,SpriteComponent>)ImGui::Text("%zu definitions, %zu instances",state.definitions.size(),state.instances.size());
                    else if constexpr(std::is_same_v<T,BitmapComponent>||std::is_same_v<T,MultiBitmapComponent>)ImGui::Text("%zu indexed images",state.items.size());
                    else if constexpr(std::is_same_v<T,SoundComponent>){for(const auto& sound:state.items){ImGui::Text("%s (%zu bytes)",sound.name.c_str(),sound.wav.size());ImGui::SameLine();ImGui::PushID(sound.name.c_str());if(ImGui::SmallButton("Play"))PlaySoundA(reinterpret_cast<LPCSTR>(sound.wav.data()),nullptr,SND_MEMORY|SND_ASYNC|SND_NODEFAULT);ImGui::PopID();}}
                    else if constexpr(std::is_same_v<T,VideoComponent>)for(auto& video:state.items){editComponent([&]{return inputText("AVI",video.fileName);});editComponent([&]{return ImGui::InputInt("X",&video.x);});editComponent([&]{return ImGui::InputInt("Y",&video.y);});}
                    else if constexpr(std::is_same_v<T,OpaqueComponent>)ImGui::Text("Preserved private bytes: %zu",state.privateState.size());},component.state);}}
        ImGui::End();return;}if(p->immutablePassthrough){ImGui::TextUnformatted("Immutable passthrough page (unknown component boundary)");ImGui::End();return;}auto edit=[&](auto fn){auto before=app.project;if(fn())app.commit(std::move(before));};
    ImGui::TextUnformatted("Page");edit([&]{return inputText("Filename",p->fileName);});edit([&]{return ImGui::Checkbox("Listed in START.PRJ",&p->listedInManifest);});int rect[4]={p->rect.left,p->rect.top,p->rect.right,p->rect.bottom};edit([&]{if(!ImGui::InputInt4("Rect",rect))return false;p=app.page();p->rect={rect[0],rect[1],rect[2],rect[3]};return true;});
    edit([&]{bool inherited=p->usesProjectPalette;if(!ImGui::Checkbox("Use project palette",&inherited))return false;p=app.page();if(p->usesProjectPalette&&!inherited)p->palette=app.project.palette;p->usesProjectPalette=inherited;return true;});
    int backgroundIndex=p->backgroundColorIndex;edit([&]{if(!ImGui::SliderInt("Background palette index",&backgroundIndex,0,255))return false;app.page()->backgroundColorIndex=static_cast<std::uint8_t>(backgroundIndex);return true;});
    if(!p->usesProjectPalette){ImGui::SeparatorText("Page palette override");ImGui::SliderInt("Page colour index",&app.paletteIndex,0,255);auto& pageColor=p->palette[static_cast<std::size_t>(app.paletteIndex)];float pageRgb[3]={pageColor.r/255.0f,pageColor.g/255.0f,pageColor.b/255.0f};
        if(ImGui::ColorEdit3("Page colour",pageRgb)){auto before=app.project;auto& changed=app.page()->palette[static_cast<std::size_t>(app.paletteIndex)];changed.r=static_cast<std::uint8_t>(pageRgb[0]*255+0.5f);changed.g=static_cast<std::uint8_t>(pageRgb[1]*255+0.5f);changed.b=static_cast<std::uint8_t>(pageRgb[2]*255+0.5f);app.requantizePagePaletteAssets();app.commit(std::move(before));}
        if(ImGui::Button("Copy shared palette")){auto before=app.project;app.page()->palette=app.project.palette;app.requantizePagePaletteAssets();app.commit(std::move(before));}ImGui::SameLine();
        const bool canRegenerate=p->background&&p->background->sourceRgba.size()==static_cast<std::size_t>(p->background->width)*p->background->height*4;if(!canRegenerate)ImGui::BeginDisabled();if(ImGui::Button("Median-cut from background")){auto before=app.project;RgbaImage source{p->background->width,p->background->height,p->background->sourceRgba};app.page()->palette=generatePalette({source},p->background->transparentIndex);app.requantizePagePaletteAssets();app.commit(std::move(before));}if(!canRegenerate)ImGui::EndDisabled();}
    ImGui::SeparatorText("Add holder");if(ImGui::Button("HotSpot"))app.addHolder(HolderKind::HotSpotHolder);ImGui::SameLine();if(ImGui::Button("Sprite"))app.addHolder(HolderKind::SpriteHolder);ImGui::SameLine();if(ImGui::Button("Bitmap"))app.addHolder(HolderKind::BitmapHolder);
    if(ImGui::Button("MultiBitmap"))app.addHolder(HolderKind::MultiBitmap);ImGui::SameLine();if(ImGui::Button("Sound"))app.addHolder(HolderKind::SoundHolder);ImGui::SameLine();if(ImGui::Button("Video"))app.addHolder(HolderKind::VideoHolder);
    ImGui::SeparatorText("Components");for(std::size_t i=0;i<p->components.size();++i){auto& c=p->components[i];const bool selected=app.selection.component==static_cast<int>(i);
        if(ImGui::Selectable((c.wrapper.displayName+(c.editable?"":" [preserved]")).c_str(),selected))app.selectOnly({static_cast<int>(i),-1});}
    if(app.selection.component>=0&&app.selection.component<static_cast<int>(p->components.size())){auto& c=p->components[app.selection.component];ImGui::SeparatorText(c.wrapper.displayName.c_str());
        {auto before=app.project;bool changed=false;changed|=ImGui::Checkbox("Editor visible",&c.editorVisible);ImGui::SameLine();changed|=ImGui::Checkbox("Editor locked",&c.editorLocked);if(changed)app.commitEditorState(std::move(before));}
        bool reordered=false;if(ImGui::Button("Component up")&&app.selection.component>0){auto before=app.project;auto* current=app.page();std::swap(current->components[app.selection.component],current->components[app.selection.component-1]);--app.selection.component;app.multiSelection=app.selection.item>=0?std::vector<Selection>{app.selection}:std::vector<Selection>{};app.commitStructure(std::move(before));reordered=true;}ImGui::SameLine();
        if(ImGui::Button("Component down")&&app.selection.component+1<static_cast<int>(p->components.size())){auto before=app.project;auto* current=app.page();std::swap(current->components[app.selection.component],current->components[app.selection.component+1]);++app.selection.component;app.multiSelection=app.selection.item>=0?std::vector<Selection>{app.selection}:std::vector<Selection>{};app.commitStructure(std::move(before));reordered=true;}if(reordered){ImGui::End();return;}
        if(app.selection.item>=0){if(ImGui::Button("Item earlier")){app.reorderSelectedItem(-1);ImGui::End();return;}ImGui::SameLine();if(ImGui::Button("Item later")){app.reorderSelectedItem(1);ImGui::End();return;}}
        if(!c.editable){ImGui::Text("Preserved read-only payload: %zu bytes",c.sourcePrivateState.size());ImGui::End();return;}
        auto componentEdit=[&](auto fn){auto before=app.project;if(fn()){app.page()->components[app.selection.component].privateDirty=true;app.commit(std::move(before));}};
        std::visit([&](auto& state){using T=std::decay_t<decltype(state)>;
            if constexpr(std::is_same_v<T,HotSpotComponent>){if(ImGui::Button("Add hotspot")){auto before=app.project;int id=0;for(const auto& x:state.items)id=std::max(id,x.id+1);state.items.push_back({0,0,64,64,id,0,true,""});app.page()->components[app.selection.component].privateDirty=true;app.selectOnly({app.selection.component,static_cast<int>(state.items.size()-1)});app.commit(std::move(before));}
                for(std::size_t i=0;i<state.items.size();++i)if(ImGui::Selectable(("Hotspot "+std::to_string(state.items[i].id)).c_str(),app.selection.item==static_cast<int>(i)))app.selectOnly({app.selection.component,static_cast<int>(i)});
                if(app.selection.item>=0&&app.selection.item<static_cast<int>(state.items.size())){auto& x=state.items[app.selection.item];componentEdit([&]{return inputText("Name",x.name);});int r[4]={x.left,x.top,x.right,x.bottom};componentEdit([&]{if(!ImGui::InputInt4("Bounds",r))return false;auto& v=std::get<HotSpotComponent>(app.page()->components[app.selection.component].state).items[app.selection.item];v.left=r[0];v.top=r[1];v.right=r[2];v.bottom=r[3];return true;});componentEdit([&]{return ImGui::InputInt("ID",&x.id);});componentEdit([&]{return ImGui::InputInt("Layer",&x.layer);});componentEdit([&]{return ImGui::Checkbox("Enabled",&x.enabled);});}}
            else if constexpr(std::is_same_v<T,SpriteComponent>){ImGui::Text("Definitions %zu, instances %zu",state.definitions.size(),state.instances.size());if(ImGui::Button("Import PNG/GIF frames"))app.importSprite();
                for(std::size_t i=0;i<state.instances.size();++i)if(ImGui::Selectable(("Sprite "+std::to_string(i)).c_str(),app.selection.item==static_cast<int>(i)))app.selectOnly({app.selection.component,static_cast<int>(i)});
                if(app.selection.item>=0&&app.selection.item<static_cast<int>(state.instances.size())){auto& x=state.instances[app.selection.item];componentEdit([&]{return ImGui::InputInt("X",&x.x);});componentEdit([&]{return ImGui::InputInt("Y",&x.y);});componentEdit([&]{return ImGui::InputInt("Layer",&x.layer);});componentEdit([&]{return ImGui::InputInt("Phase",&x.phase);});componentEdit([&]{return ImGui::Checkbox("Visible",&x.visible);});componentEdit([&]{return ImGui::Checkbox("Draggable",&x.draggable);});
                    if(x.definitionIndex<state.definitions.size()){auto& definition=state.definitions[x.definitionIndex];ImGui::SeparatorText("Sprite definition / frames");componentEdit([&]{return inputText("Definition name",definition.name);});componentEdit([&]{return ImGui::Checkbox("Definition pixel hit-test",&definition.pixelHitTest);});if(!definition.frames.empty()){app.spriteFrameIndex=std::clamp(app.spriteFrameIndex,0,static_cast<int>(definition.frames.size())-1);ImGui::SliderInt("Frame",&app.spriteFrameIndex,0,static_cast<int>(definition.frames.size())-1);auto& frame=definition.frames[app.spriteFrameIndex];int duration=static_cast<int>(frame.durationMs);componentEdit([&]{if(!ImGui::InputInt("Frame duration ms",&duration))return false;auto& current=std::get<SpriteComponent>(app.page()->components[app.selection.component].state).definitions[x.definitionIndex].frames[app.spriteFrameIndex];current.durationMs=static_cast<std::uint32_t>(std::max(duration,1));return true;});componentEdit([&]{return ImGui::Checkbox("Frame pixel hit-test",&frame.pixelHitTest);});int transparent=frame.image.transparentIndex;componentEdit([&]{if(!ImGui::SliderInt("Frame transparent index",&transparent,0,255))return false;std::get<SpriteComponent>(app.page()->components[app.selection.component].state).definitions[x.definitionIndex].frames[app.spriteFrameIndex].image.transparentIndex=static_cast<std::uint8_t>(transparent);return true;});ImGui::Text("%ux%u; RGBA source: %s",frame.image.width,frame.image.height,frame.image.sourceRgba.empty()?"legacy indexed only":"retained");}}}}
            else if constexpr(std::is_same_v<T,BitmapComponent>){if(ImGui::Button("Import bitmap"))app.importBitmap(false);for(std::size_t i=0;i<state.items.size();++i)if(ImGui::Selectable(state.items[i].name.c_str(),app.selection.item==static_cast<int>(i)))app.selectOnly({app.selection.component,static_cast<int>(i)});
                if(app.selection.item>=0&&app.selection.item<static_cast<int>(state.items.size())){auto& x=state.items[app.selection.item];componentEdit([&]{return inputText("Name",x.name);});componentEdit([&]{return ImGui::InputInt("X",&x.x);});componentEdit([&]{return ImGui::InputInt("Y",&x.y);});componentEdit([&]{return ImGui::InputInt("Layer",&x.layer);});componentEdit([&]{return ImGui::Checkbox("Initially hidden",&x.initiallyHidden);});componentEdit([&]{return ImGui::Checkbox("Pixel hit-test",&x.pixelHitTest);});int transparent=x.image.transparentIndex;componentEdit([&]{if(!ImGui::SliderInt("Transparent index",&transparent,0,255))return false;std::get<BitmapComponent>(app.page()->components[app.selection.component].state).items[app.selection.item].image.transparentIndex=static_cast<std::uint8_t>(transparent);return true;});ImGui::Text("%ux%u; RGBA source: %s",x.image.width,x.image.height,x.image.sourceRgba.empty()?"legacy indexed only":"retained");}}
            else if constexpr(std::is_same_v<T,MultiBitmapComponent>){if(ImGui::Button("Import multibitmap"))app.importBitmap(true);for(std::size_t i=0;i<state.items.size();++i)if(ImGui::Selectable(state.items[i].name.c_str(),app.selection.item==static_cast<int>(i)))app.selectOnly({app.selection.component,static_cast<int>(i)});if(app.selection.item>=0&&app.selection.item<static_cast<int>(state.items.size())){auto& x=state.items[app.selection.item];componentEdit([&]{return inputText("Name",x.name);});componentEdit([&]{return ImGui::InputInt("X",&x.x);});componentEdit([&]{return ImGui::InputInt("Y",&x.y);});componentEdit([&]{return ImGui::InputInt("Layer",&x.layer);});componentEdit([&]{return ImGui::Checkbox("Shown",&x.shown);});int transparent=x.image.transparentIndex;componentEdit([&]{if(!ImGui::SliderInt("Transparent index",&transparent,0,255))return false;std::get<MultiBitmapComponent>(app.page()->components[app.selection.component].state).items[app.selection.item].image.transparentIndex=static_cast<std::uint8_t>(transparent);return true;});ImGui::Text("%ux%u",x.image.width,x.image.height);}}
            else if constexpr(std::is_same_v<T,SoundComponent>){if(ImGui::Button("Import WAV"))app.importSound();ImGui::SameLine();if(ImGui::Button("Stop preview"))PlaySoundW(nullptr,nullptr,0);for(std::size_t i=0;i<state.items.size();++i)if(ImGui::Selectable((std::to_string(i)+": "+state.items[i].name).c_str(),app.selection.item==static_cast<int>(i)))app.selectOnly({app.selection.component,static_cast<int>(i)});if(app.selection.item>=0&&app.selection.item<static_cast<int>(state.items.size())){auto& sound=state.items[app.selection.item];componentEdit([&]{return inputText("Sound name",sound.name);});if(ImGui::Button("Play WAV"))PlaySoundA(reinterpret_cast<LPCSTR>(sound.wav.data()),nullptr,SND_MEMORY|SND_ASYNC|SND_NODEFAULT);ImGui::Text("Embedded RIFF bytes: %zu",sound.wav.size());}}
            else if constexpr(std::is_same_v<T,VideoComponent>){if(ImGui::Button("Import AVI"))app.importVideo();for(std::size_t i=0;i<state.items.size();++i)if(ImGui::Selectable(state.items[i].fileName.c_str(),app.selection.item==static_cast<int>(i)))app.selectOnly({app.selection.component,static_cast<int>(i)});if(app.selection.item>=0&&app.selection.item<static_cast<int>(state.items.size())){auto& video=state.items[app.selection.item];componentEdit([&]{const auto old=video.fileName;if(!inputText("AVI filename",video.fileName))return false;for(auto& asset:app.project.looseAssets)if(asset.fileName==old){asset.fileName=video.fileName;break;}return true;});componentEdit([&]{return ImGui::InputInt("X",&video.x);});componentEdit([&]{return ImGui::InputInt("Y",&video.y);});componentEdit([&]{return ImGui::InputInt("Preview width",&video.previewWidth);});componentEdit([&]{return ImGui::InputInt("Preview height",&video.previewHeight);});if(ImGui::Button("Decode preview frame"))app.previewVideoAsset(video.fileName);}}
            else if constexpr(std::is_same_v<T,OpaqueComponent>)ImGui::Text("Preserved read-only payload: %zu bytes",state.privateState.size());
        },c.state);}
    ImGui::End();}

void drawAssets(App& app){ImGui::Begin("Assets");if(ImGui::Button("Background"))app.importBackground(false);ImGui::SameLine();if(ImGui::Button("Background + median-cut palette"))app.importBackground(true);
    ImGui::SliderInt("Import transparent index",&app.importTransparentIndex,0,255);ImGui::SliderInt("Alpha cutoff",&app.alphaCutoff,0,255);
    if(ImGui::Button("Sprite / GIF"))app.importSprite();ImGui::SameLine();if(ImGui::Button("Bitmap"))app.importBitmap(false);ImGui::SameLine();if(ImGui::Button("MultiBitmap"))app.importBitmap(true);
    if(ImGui::Button("WAV"))app.importSound();ImGui::SameLine();if(ImGui::Button("AVI"))app.importVideo();ImGui::SameLine();if(ImGui::Button("Cursor")&&app.group())app.importCursor();ImGui::SameLine();if(ImGui::Button("Package loose file"))app.importLooseAsset();
    ImGui::SeparatorText("Shared palette");if(ImGui::Button("Reset RGB332")){auto before=app.project;app.project.palette=makeRgb332Palette();app.requantizeProjectPaletteAssets();app.commit(std::move(before));}
    ImGui::SliderInt("Index",&app.paletteIndex,0,255);auto& color=app.project.palette[static_cast<std::size_t>(app.paletteIndex)];float rgb[3]={color.r/255.0f,color.g/255.0f,color.b/255.0f};
    if(ImGui::ColorEdit3("Colour",rgb)){auto before=app.project;auto& edited=app.project.palette[static_cast<std::size_t>(app.paletteIndex)];edited.r=static_cast<std::uint8_t>(rgb[0]*255+0.5f);edited.g=static_cast<std::uint8_t>(rgb[1]*255+0.5f);edited.b=static_cast<std::uint8_t>(rgb[2]*255+0.5f);app.requantizeProjectPaletteAssets();app.commit(std::move(before));}
    if(!app.videoPreviewStatus.empty())ImGui::TextWrapped("%s",app.videoPreviewStatus.c_str());if(app.videoTexture.view){const float scale=std::min(1.0f,320.0f/std::max(1,app.videoTexture.width));ImGui::Image(reinterpret_cast<ImTextureID>(app.videoTexture.view),{app.videoTexture.width*scale,app.videoTexture.height*scale});}
    ImGui::Text("Loose packaged assets: %zu",app.project.looseAssets.size());for(const auto& a:app.project.looseAssets)ImGui::BulletText("%s (%zu bytes)",a.fileName.c_str(),a.bytes.size());ImGui::End();}

void drawScript(App& app){ImGui::Begin("Script");if(ImGui::RadioButton("Page script",!app.editGlobalScript))app.editGlobalScript=false;ImGui::SameLine();if(ImGui::RadioButton("Global setup",app.editGlobalScript))app.editGlobalScript=true;auto* p=app.page();
    if(app.editGlobalScript){if(ImGui::Button("Insert global variable")){auto before=app.project;app.project.globalScript+="\r\nint global1=0;\r\n";app.commitEditorState(std::move(before));}auto before=app.project;if(inputTextMultiline("##global",app.project.globalScript,ImGui::GetContentRegionAvail()))app.commitEditorState(std::move(before));ImGui::End();return;}
    if(!p){ImGui::TextUnformatted("Select a page to edit its script.");ImGui::End();return;}if(p->immutablePassthrough){ImGui::TextUnformatted("Script is locked because this page is an immutable passthrough.");ImGui::End();return;}if(ImGui::Button("Insert page event template")){auto before=app.project;p->script+="\r\nvoid OnTimer(int timerID)\r\n{\r\n}\r\n";app.commit(std::move(before));}
    ImGui::SameLine();if(ImGui::Button("Insert selected callback")&&app.selection.component>=0&&app.selection.component<static_cast<int>(p->components.size())){const auto& c=p->components[app.selection.component];auto before=app.project;
        for(const auto& fn:c.wrapper.functions)if(fn.displayName!="QueryInterface"&&fn.displayName!="AddRef"&&fn.displayName!="Release"&&fn.displayName!="GetTypeInfo"&&fn.displayName!="GetTypeInfoCount"&&fn.displayName!="GetIDsOfNames"&&fn.displayName!="Invoke"){
            p->script+="\r\nvoid "+c.wrapper.displayName+"."+fn.displayName+fn.descriptionOrHelpName+"\r\n{\r\n}\r\n";break;}app.commit(std::move(before));}
    static const char* api[]={"LoadPage(\"PAGE.BDF\");","LoadGroup(\"CURSORS.GRP\");","SetCursor(0);","SetTimer(1000);","Sound_Holder.PlayDSound(0);","Sprite_Holder.ShowSprite(0);","Bitmap_Holder.ShowBitmap(0);","Video_Holder.Play(0);"};static int apiIndex=0;
    ImGui::SetNextItemWidth(240);ImGui::Combo("Known API",&apiIndex,api,IM_ARRAYSIZE(api));ImGui::SameLine();if(ImGui::Button("Insert API")){auto before=app.project;p->script+="\r\n   "+std::string(api[apiIndex]);app.commit(std::move(before));}
    if(ImGui::CollapsingHeader("References / navigation")){std::set<std::string> shown;for(const auto& call:runtime::collectCalls(p->script))if(!call.isBuiltin&&shown.insert(call.component).second){ImGui::PushID(call.component.c_str());if(ImGui::SmallButton("Go"))app.navigateToObject(p->fileName+":"+call.component);ImGui::SameLine();ImGui::Text("%s.%s",call.component.c_str(),call.name.c_str());ImGui::PopID();}const auto tokens=runtime::tokenize(p->script);for(std::size_t i=0;i+2<tokens.size();++i)if(tokens[i].kind==runtime::TokenKind::Identifier&&(tokens[i].text=="LoadPage"||tokens[i].text=="LoadGroup")&&tokens[i+2].kind==runtime::TokenKind::String){ImGui::PushID(static_cast<int>(i));if(ImGui::SmallButton("Go"))app.navigateToObject(tokens[i+2].text);ImGui::SameLine();ImGui::Text("%s -> %s",tokens[i].text.c_str(),tokens[i+2].text.c_str());ImGui::PopID();}}
    auto before=app.project;if(inputTextMultiline("##script",p->script,ImGui::GetContentRegionAvail()))app.commit(std::move(before));ImGui::End();}

void drawDiagnostics(App& app){ImGui::Begin("Diagnostics");const auto diagnostics=validateProject(app.project);if(diagnostics.empty())ImGui::TextColored({0.4f,1,0.4f,1},"No validation errors");
    for(std::size_t i=0;i<diagnostics.size();++i){const auto& d=diagnostics[i];const ImVec4 color=d.severity==DiagnosticSeverity::Error?ImVec4(1,.3f,.3f,1):ImVec4(1,.8f,.3f,1);ImGui::PushID(static_cast<int>(i));ImGui::PushStyleColor(ImGuiCol_Text,color);const auto label="["+d.code+"] "+d.object+": "+d.message;if(ImGui::Selectable(label.c_str()))app.navigateToObject(d.object);ImGui::PopStyleColor();ImGui::PopID();}ImGui::End();}

App* gApp=nullptr;
LRESULT WINAPI wndProc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam){if(ImGui_ImplWin32_WndProcHandler(hwnd,msg,wParam,lParam))return true;switch(msg){case WM_SIZE:if(gDevice&&wParam!=SIZE_MINIMIZED){cleanupRenderTarget();gSwapChain->ResizeBuffers(0,LOWORD(lParam),HIWORD(lParam),DXGI_FORMAT_UNKNOWN,0);createRenderTarget();}return 0;
    case WM_CLOSE:if(gApp&&!gApp->confirmDiscard())return 0;DestroyWindow(hwnd);return 0;case WM_SYSCOMMAND:if((wParam&0xfff0)==SC_KEYMENU)return 0;break;case WM_DESTROY:PostQuitMessage(0);return 0;}return DefWindowProcW(hwnd,msg,wParam,lParam);}

} // namespace

int WINAPI wWinMain(HINSTANCE instance,HINSTANCE,PWSTR,int show){CoInitializeEx(nullptr,COINIT_APARTMENTTHREADED);const bool mediaFoundation=SUCCEEDED(MFStartup(MF_VERSION,MFSTARTUP_LITE));WNDCLASSEXW wc{sizeof(wc),CS_CLASSDC,wndProc,0,0,instance,nullptr,nullptr,nullptr,nullptr,L"GraphBoardStudio",nullptr};
    RegisterClassExW(&wc);HWND hwnd=CreateWindowW(wc.lpszClassName,L"GraphBoard Studio",WS_OVERLAPPEDWINDOW,100,100,1500,900,nullptr,nullptr,wc.hInstance,nullptr);
    if(!createDevice(hwnd)){cleanupDevice();UnregisterClassW(wc.lpszClassName,wc.hInstance);if(mediaFoundation)MFShutdown();CoUninitialize();return 1;}ShowWindow(hwnd,show);UpdateWindow(hwnd);
    IMGUI_CHECKVERSION();ImGui::CreateContext();ImGuiIO& io=ImGui::GetIO();io.ConfigFlags|=ImGuiConfigFlags_NavEnableKeyboard|ImGuiConfigFlags_DockingEnable;ImGui::StyleColorsDark();
    ImGui_ImplWin32_Init(hwnd);ImGui_ImplDX11_Init(gDevice,gContext);App app;gApp=&app;bool done=false;
    while(!done){MSG msg;while(PeekMessage(&msg,nullptr,0,0,PM_REMOVE)){TranslateMessage(&msg);DispatchMessage(&msg);if(msg.message==WM_QUIT)done=true;}if(done)break;
        ImGui_ImplDX11_NewFrame();ImGui_ImplWin32_NewFrame();ImGui::NewFrame();ImGui::DockSpaceOverViewport();drawMenu(app);
        const bool control=(GetAsyncKeyState(VK_CONTROL)&0x8000)!=0;if(control&&ImGui::IsKeyPressed(ImGuiKey_Z,false))app.undoOnce();if(control&&ImGui::IsKeyPressed(ImGuiKey_Y,false))app.redoOnce();if(control&&ImGui::IsKeyPressed(ImGuiKey_S,false))app.saveProjectNow();
        if(control&&ImGui::IsKeyPressed(ImGuiKey_N,false)&&app.confirmDiscard())app.newProject();if(control&&ImGui::IsKeyPressed(ImGuiKey_O,false)&&app.confirmDiscard()){if(auto path=fileDialog(false,L"GraphBoard project\0*.gbproj\0All files\0*.*\0"))try{app.openProject(*path);}catch(const std::exception& e){app.status=e.what();}}
        drawProject(app);drawCanvas(app);drawInspector(app);drawAssets(app);drawScript(app);drawDiagnostics(app);
        if(app.projectDirty&&std::chrono::steady_clock::now()-app.lastAutosave>std::chrono::seconds(30)){try{saveProject(app.autosaveDirectory(),app.project);}catch(...){ }app.lastAutosave=std::chrono::steady_clock::now();}
        ImGui::Render();const float clear[4]={0.08f,0.08f,0.09f,1};gContext->OMSetRenderTargets(1,&gRenderTarget,nullptr);gContext->ClearRenderTargetView(gRenderTarget,clear);ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());gSwapChain->Present(1,0);}
    gApp=nullptr;ImGui_ImplDX11_Shutdown();ImGui_ImplWin32_Shutdown();ImGui::DestroyContext();cleanupDevice();if(IsWindow(hwnd))DestroyWindow(hwnd);UnregisterClassW(wc.lpszClassName,wc.hInstance);if(mediaFoundation)MFShutdown();CoUninitialize();return 0;}
