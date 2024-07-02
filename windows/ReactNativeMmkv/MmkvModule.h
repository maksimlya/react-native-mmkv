#pragma once

#include "JSValue.h"
#include <jsi/jsi.h>
#include "NativeModules.h"
#include <JSI/JsiApiContext.h>
#include <winrt/Windows.Storage.h>
#include <winrt/Microsoft.ReactNative.h>
#include <TurboModuleProvider.h>
#include <MMKV/MMKV.h>
#include <string>
#include <optional>
#include "TypedArray.h"
#include "MmkvHostObject.h"


using namespace winrt::Microsoft::ReactNative;
using namespace facebook;
using namespace winrt;
using namespace Windows::Storage;


namespace winrt::ReactNativeMmkv
{

REACT_MODULE(MMKVModule, L"MMKV")
struct MMKVModule
{
    // See https://microsoft.github.io/react-native-windows/docs/native-modules for details on writing native modules

    REACT_INIT(Initialize)
    void Initialize(ReactContext const &reactContext) noexcept
    {
        m_reactContext = reactContext;
    }

    // Function to get property as std::string
    std::string getPropertyAsStringOrNilFromObject(jsi::Object& object,
                                                   const std::string& propertyName,
                                                   jsi::Runtime& runtime) {

        jsi::Value value = object.getProperty(runtime, propertyName.c_str());

        if (value.isString()) {
          return value.asString(runtime).utf8(runtime);
        }
        return "";
    }
    
    REACT_SYNC_METHOD(install)
    bool install(std::string unusedStringArg) noexcept
    {

        if (m_reactContext == nullptr) {
          return false;
        }
        jsi::Runtime* jsiRuntime = TryGetOrCreateContextRuntime(m_reactContext);

        if (jsiRuntime == nullptr) {
          return false;
        }

        auto& runtime = *jsiRuntime;

        winrt::Windows::Storage::StorageFolder localFolder =
            winrt::Windows::Storage::ApplicationData::Current().LocalFolder();

        // Check if the folder exists and create it if it doesn't.
        winrt::Windows::Storage::StorageFolder mmkvFolder = nullptr;
        try {
          // Try to get the folder.
          mmkvFolder = localFolder.GetFolderAsync(L"mmkv").get();
        } catch (const winrt::hresult_error& ex) {
          // If the folder doesn't exist, create it.
            mmkvFolder = localFolder.CreateFolderAsync(L"mmkv").get();
        }

        MMKVLogLevel logLevel = MMKVLogError; // TODO ?
        MMKV::initializeMMKV(mmkvFolder.Path().c_str(), logLevel);
        
        // MMKV.createNewInstance()
        auto mmkvCreateNewInstance = jsi::Function::createFromHostFunction(
            runtime, jsi::PropNameID::forAscii(runtime, "mmkvCreateNewInstance"), 1,
            [this](jsi::Runtime& runtime, const jsi::Value& thisValue, const jsi::Value* arguments,
                size_t count) -> jsi::Value {
                if (count != 1) {
                throw jsi::JSError(runtime,
                                    "MMKV.createNewInstance(..) expects one argument (object)!");
                }
                jsi::Object config = arguments[0].asObject(runtime);

                std::string instanceId = getPropertyAsStringOrNilFromObject(config, "id", runtime);
                std::string path = getPropertyAsStringOrNilFromObject(config, "path", runtime);
                std::string encryptionKey = getPropertyAsStringOrNilFromObject(config, "encryptionKey", runtime);

                auto instance = std::make_shared<MmkvHostObject>(instanceId, path, encryptionKey);
                return jsi::Object::createFromHostObject(runtime, instance);
            });
        runtime.global().setProperty(runtime, "mmkvCreateNewInstance",
                                        std::move(mmkvCreateNewInstance));

        // Adds the PropNameIDCache object to the Runtime. If the Runtime gets destroyed, the Object
        // gets destroyed and the cache gets invalidated.
        auto propNameIdCache = std::make_shared<InvalidateCacheOnDestroy>(runtime);
        runtime.global().setProperty(runtime, "mmkvArrayBufferPropNameIdCache",
                                         jsi::Object::createFromHostObject(runtime, propNameIdCache));
       return true;
    }

    private:
        ReactContext m_reactContext{nullptr};
};

} // namespace winrt::ReactNativeMmkv
