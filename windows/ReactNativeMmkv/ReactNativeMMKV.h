#pragma once

#include "JSValue.h"
#include <jsi/jsi.h>
#include "NativeModules.h"
#include <JSI/JsiApiContext.h>
#include <winrt/Microsoft.ReactNative.h>
#include <TurboModuleProvider.h>

using namespace winrt::Microsoft::ReactNative;

namespace winrt::ReactNativeMmkv
{

REACT_MODULE(MMKV, L"MMKV")
struct MMKV
{
    // See https://microsoft.github.io/react-native-windows/docs/native-modules for details on writing native modules

    REACT_INIT(Initialize)
    void Initialize(ReactContext const &reactContext) noexcept
    {
        m_reactContext = reactContext;
    }
    
    REACT_SYNC_METHOD(install)
    bool install(std::string stringArgument) noexcept
    {
       // TODO.....
       return true;
    }

    private:
        ReactContext m_reactContext{nullptr};
};

} // namespace winrt::ReactNativeMmkv
