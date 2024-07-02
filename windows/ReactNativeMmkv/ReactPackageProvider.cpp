#include "pch.h"
#include "ReactPackageProvider.h"
#if __has_include("ReactPackageProvider.g.cpp")
#include "ReactPackageProvider.g.cpp"
#endif

#include "MmkvModule.h"

using namespace winrt::Microsoft::ReactNative;

namespace winrt::ReactNativeMmkv::implementation
{

void ReactPackageProvider::CreatePackage(IReactPackageBuilder const &packageBuilder) noexcept
{
    AddAttributedModules(packageBuilder, false);
}

} // namespace winrt::ReactNativeMmkv::implementation
