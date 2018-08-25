#pragma once

/* Standard libraries: */
#include <vector>
#include <string>

/* Temporary testing: */
#define _USE_MATH_DEFINES
#include <cmath>

/* Windows libraries: */
// For more info see doc/WASAPI_notes.md
// Component Object Model:
#include <objbase.h>
// Multimedia Device API:
#include <mmdeviceapi.h>
// Audio Client:
#include <audioclient.h>

/* Fix missing PKEYs:

These were supposed to be declared in the global scope,
but for some reason they are undefined with my compiler.
TODO: I will keep them in the global scope for now,
but some more elegant solution would be nice. */
extern PROPERTYKEY PKEY_Device_FriendlyName;

namespace Cynth {

/* -- Temporary testing: ---------------------------------------------------- */

    float sin_sub(float t);

    long sin_tmp(long max, float freq, float t);
    //float sin_minmax_Hz(float min, float max, float freq, float t);

    class Sample {
    private:
        std::vector<unsigned char> bytes;
    
    public:
        /* Constructors: */
        Sample();
        Sample(int bit_depth);

        /* Mutators: */
        Sample& operator=(long value); // TODO: Check overflow
        Sample& operator=(unsigned long value);

        /* Accessors: */
        unsigned char operator[](int i);
        unsigned char* data();
        std::size_t size();
    };

    class Buffer {
    private:
        std::vector<unsigned char> bytes;
        int bit_depth;

    public:
        Buffer(int bit_depth);

        void write(Sample sample);
        unsigned char* data();
        std::size_t size();
        void clear();
        void copyTo(unsigned char* ptr_buffer);
        void moveTo(unsigned char* ptr_buffer);
    };

/* -------------------------------------------------------------------------- */

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

            /* Move & Copy: */
            // Copy constructor:
            Interface(const Interface<interface_t>& other);
            // Copy assignment:
            Interface<interface_t>& operator=(const Interface<interface_t>& other);
            // Move constructor:
            Interface(Interface<interface_t>&& other);
            // Move assignment:
            Interface<interface_t>& operator=(Interface<interface_t>&& other);

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
            // Move/copy/default constructor and move/copy assignment
            // are not inherited with explicit destructor:
            PropertyStore();
            PropertyStore(IPropertyStore* ptr_instance);
            PropertyStore(const PropertyStore& other);
            PropertyStore(PropertyStore&& other);
            PropertyStore& operator=(const PropertyStore& other);
            PropertyStore& operator=(PropertyStore&& other);

            /* WASAPI methods abstractions: */
            prop_t& getValue(PROPERTYKEY);
        };

        class RenderClient: public Interface<IAudioRenderClient> {public:
            /* Constructor: */
            using Interface<IAudioRenderClient>::Interface;

            /* Friends: */
            friend class Device;
            friend class AudioClient;
        };

        class AudioClient: public Interface<IAudioClient> {
        public:
            /* Constructor: */
            using Interface<IAudioClient>::Interface;

            /* Friends: */
            friend class Device;

            /* Related WASAPI interfaces: */
            RenderClient render_client;
        };

        /*/ Device: /*/
        class Device: public Interface<IMMDevice> {
        private:
            /* Properties: */
            // Display name:
            std::string name;

            /* WASAPI properties: */
            // TODO: Type aliases needed?
            REFERENCE_TIME default_device_period;
            REFERENCE_TIME minimum_device_period;
            // WAVFORMATEX may contain extra bytes after
            // to form WAVEFORMATEXTENSIBLE. Number of these extra bytes
            // is stored in wave_format->cbSize. The extra information
            // may be needed for further WASAPI methods.
            // So as not to copy the information manually from memory,
            // only a pointer is stored to the received WAVEFORMATEX,
            // which is followed by these potential extra bytes.
            // TODO: For cleanup, is it needed to delete these extra bytes?
            WAVEFORMATEX* wave_format;
            UINT32 buffer_size_frames;
            INT32 buffer_size_bytes;

            /* WASAPI methods abstractions: */
            void getDevicePeriod();
            void getBufferSize();
            void getMixFormat();
            void checkFormatSupport();
            AudioClient getAudioClient();
            void initAudioClient();
            RenderClient getRenderClient();

        public:
            /* Constructor: */
            Device(IMMDevice* ptr_instance = nullptr);

            /* Audio Client: */
            // Activate and setup Audio Client:
            void setup();
            // Release AudioClient:
            void releaseAudioClient();

            /* Friends: */
            friend class Control;

            /* Mutators: */
            void setName(std::string name);

            /* Related WASAPI interfaces: */
            // Audio Client:
            AudioClient audio_client;

            /* WASAPI methods abstractions: */
            PropertyStore openPropertyStore(DWORD access = STGM_READ);
            std::string getId();
        };

        /*/ Device Collection: /*/
        class DeviceCollection: public Interface<IMMDeviceCollection> {
        public:
            /* Constructor: */
            using Interface<IMMDeviceCollection>::Interface;

            /* WASAPI methods abstractions: */
            int getCount();
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
            Device getDefaultAudioEndpoint(
                EDataFlow data_flow = EDataFlow::eRender,
                ERole role = ERole::eMultimedia);
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

            /* Mutators: */
            void clearRenderingDevices();

        public:
            /* Constructor:
            
            Initializes COM library.
            Gets all available rendering devices from IMMDeviceEnumerator
            and saves them in rendering_devices vector.
            Saves the default rendering device in active_rendering_devices
            vector. */
            Control();

            /* Fix missing PKEYs: */
            void fixMissingPKEYs();

            /* Accessors: */
            // Get available devices:
            std::vector<std::string> getRenderingDevicesNames();
            int getRenderingDevicesCount();

            /* Mutators: */
            // Set active rendering devices:
            void setRenderingDevices(std::vector<int> ids);
            void setRenderingDevice(int id);
            void addRenderingDevice(int id);
        };

    }

}