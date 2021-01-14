#include "parser.h"
#include "resource.h"

using namespace Parser;

File::File(ConfigResource* resource, std::string name)
: resource(resource),
  name(name)
{
    auto pkg = resource->GetResource()->GetPackage();
    alt::IPackage::File* pkgFile = pkg->OpenFile(name);
    alt::String src(pkg->GetFileSize(pkgFile));
    pkg->ReadFile(pkgFile, src.GetData(), src.GetSize());
    pkg->CloseFile(pkgFile);

    alt::config::Parser parser(src.CStr(), src.GetSize());
    config = parser.Parse();
}

void File::Parse()
{
    try {
        // Load includes
        auto includes = config["includes"];
        ParseIncludes(includes);

        // Load every tick natives
        auto natives = config["everyTick"];
        ParseNatives(natives);
    }    
    catch(...) {}
}

void File::ParseIncludes(alt::config::Node includes)
{
    if(!includes.IsList()) return;
    for(auto include : includes.ToList())
    {
        auto name = include.ToString();
        // Read and parse the include
        resource->LoadFile(name);
    }
}

void File::ParseNatives(alt::config::Node natives)
{
    if(!natives.IsList()) return;
    // todo: Parse natives
}