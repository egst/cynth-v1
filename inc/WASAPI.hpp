#pragma once

/* Standard libraries: */
#include <vector>
#include <string>

/* Windows libraries: */
// For more info see doc/WASAPI_notes.md
// Component Object Model:
#include <objbase.h>
// Multimedia Device API:
#include <mmdeviceapi.h>

/* Fix missing PKEYs:

These were supposed to be declared in the global scope,
but for some reason they are undefined with my compiler.
TODO: I will keep them in the global scope for now,
but some more elegant solution would be nice. */
extern PROPERTYKEY PKEY_Device_FriendlyName;

namespace Cynth {

    namespace WASAPI {

        /*/ Generic Interface:
        WASAPI interfaces abstractions.
        
        These provide abstractions for COM interfaces with key features:
         * Safe to work with without managing the memory allocation manually.
         * More modern C++ approach: Avoiding out parameters etc.
         * HRESULT check is done by these abstractions.
        
        COM interfaces get released and all pointer allocated memory is freed
        when an Interface object is destructed.
        This may be suppressed with auto_release property set to false.
        Suppressing the release is used when the object is created
        on the stack, then returned by value.
        When obtaining such objects, use interface_object.autoRelease(),
        or call interface_object->Release() manually when needed.
        
        The COM interface instance is accessible via ptr_instance pointer.
        The -> operator of the Interface class is overloaded to provide
        this instance too:
        Instead of interface_object.ptr_instance->CoMethod()
        use interface_object->CoMethod().
        
        Some COM methods are wrapped in a custom abstraction method.
        These have the same names as the COM methods,
        but they follow local naming convention - camelCase:
        If there is an abstraction for interface_object->CoMethod() provided,
        use interface_object.coMethod().
        
        Some COM interface abstractions contain initialization
        with CoCreateInstance() inside their constructor.
        Some are constructed via passing the interface pointer
        (received from other abstractions) to their constructor.
        
        HRESULT is checked by these abstractions.
        When it fails, the situation is then handled by Cynth::Logger. /*/
        template<typename interface_t>
        class Interface {
        protected:
            /* Properties: */
            // WASAPI interface instance:
            interface_t* ptr_instance;
            // When false, the interface instance
            // is not released on destruction.
            bool auto_release;

        public:
            /* Constructor:
            
            Sets the ptr_instance property to the given interface pointer. */
            Interface();
            Interface(interface_t* ptr_instance);

            /* Destructor:

            Releases the WASAPI interface instance. */
            virtual ~Interface();

            // Allow auto release: (default)
            void autoRelease();
            // Suppress auto release:
            void noAutoRelease();

            /* Accessors: */
            // Allows WASAPI instance method to be used directly:
            interface_t* operator->();
        };

        /*/ Property Store: /*/
        class PropertyStore: public Interface<IPropertyStore> {
        private:
            /* PROPVARIANT abstraction: */
            struct prop_t {
                /* Destructor:
                
                Frees the memory allocated by prop_variant.
                prop_t is only returned by refference,
                to ensure control over memory deallocation. */
                // For now, memory management is moved to the PropertyStore.
                //~prop_t();

                /* Properties: */
                // TODO: Make these private,
                // and allow only the parent PropertyStore to modify these.
                PROPVARIANT prop_variant;
                std::string str_val;
            };

            prop_t property;

        public:
            /* Constructor: */
            using Interface<IPropertyStore>::Interface;

            /* Destructor:
            
            Frees the memory allocated by its value. */
            ~PropertyStore();

            /* WASAPI methods abstractions: */
            prop_t& getValue(PROPERTYKEY);
        };

        /*/ Device: /*/
        class Device: public Interface<IMMDevice> {
        private:
            /* Properties: */
            // Display name:
            std::string name;

        public:
            /* Constructor: */
            using Interface<IMMDevice>::Interface;

            /* Friemds: */
            friend class Control;

            /* Mutators: */
            void setName(std::string name);

            /* WASAPI methods abstractions: */
            PropertyStore openPropertyStore(DWORD access = STGM_READ);
        };

        /*/ Device Collection: /*/
        class DeviceCollection: public Interface<IMMDeviceCollection> {
        public:
            /* Constructor: */
            using Interface<IMMDeviceCollection>::Interface;

            /* WASAPI methods abstractions: */
            UINT getCount();
            Device item(UINT i);

            //tmp:
            IMMDeviceCollection* getPtr() {
                return this->ptr_instance;
            }
        };

        /*/ Device Enumerator: /*/
        class DeviceEnumerator: public Interface<IMMDeviceEnumerator> {
        public:
            /* Constructor:
            
            Creates WASAPI interface instance. */
            DeviceEnumerator();

            /* WASAPI methods abstractions: */
            DeviceCollection getDeviceCollection(
                EDataFlow data_flow = EDataFlow::eRender,
                DWORD mask = DEVICE_STATE_ACTIVE);
        };

        class Control {
        private:
            /* Properties: */
            // Rendering devices:
            std::vector<Device> rendering_devices;
            // Active rendering devices ids in the rendering_devices vector:
            // Any number of devices may be chosen,
            // each receives audio stream as a single channel.
            std::vector<int> active_rendering_devices;

            /* Devices interaction: */
            // Initiate Device's IMMDevice interface:
            void initDevice(int id);
            // Release Device's IMMDevice interface:
            void deinitDevice(int id);

        public:
            /* Constructor:
            
            Initializes COM library.
            Gets all available rendering devices from IMMDeviceEnumerator
            and saves them in rendering_devices vector. */
            Control();

            /* Fix missing PKEYs: */
            void fixMissingPKEYs();

            /* Accessors: */
            // Get available devices:
            std::vector<std::string> getRenderingDevicesNames();

            /* Mutators: */
            // Set active rendering devices:
            void setRenderingDevices(std::vector<int> ids);
        };

    }

}