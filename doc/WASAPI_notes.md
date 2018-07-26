# COM & WASAPI notes

## Introduction

These notes were created as my own learning notes. They are also meant for anyone reviewing this code with no knowledge of Component Object Model. Component Object Model (COM) is basically a way to use binary libraries in any programming language. It is developed by Microsoft for Windows, but may be ported to any other OS. In this project I use it only to access WASAPI - a Windows Audio API. COM library consists of separate "coclasses". Access to these is provided via their various "interfaces". Caller gets an instance of a coclass with certain interface (called "class") via the COM functions listed below.

## Note on intellisense

I develop this project in VSCode using the Microsoft C/C++ extension and its intellisense. If anyone has a simmilar setup, they might encounter some problems with the intellisense. For some reason it has problems with some parts of the code in the headers listed below. I applied a few manual fixes using a directive `#ifdef __INTELLISENSE__` which provides an option to modify some code from the headers, that the intellisense has problems with, but only for the intellisense, they do not apply when compiling the code.

### List of changes

TODO: Provide my modified header files.

    file: C:\Programs\MinGW\mingw64\x86_64-w64-mingw32\include\combaseapi.h
    on lines 41-45

    file: C:\Programs\MinGW\mingw64\x86_64-w64-mingw32\include\basetyps.h
    on lines 22-26

    file: C:\Programs\MinGW\mingw64\x86_64-w64-mingw32\include\winnt.h
    on lines 156-159 and 441-445

    file: C:\Programs\MinGW\mingw64\x86_64-w64-mingw32\include\cryptxml.h
    on lines 18-21

    file: C:\Programs\MinGW\mingw64\x86_64-w64-mingw32\include\ntdef.h
    on lines 206-209

    file: C:\Programs\MinGW\mingw64\x86_64-w64-mingw32\include\rpc.h
    on lines 66-69

    file: C:\Programs\MinGW\mingw64\x86_64-w64-mingw32\include\wincrypt.h
    on lines 27-30

    file: C:\Programs\MinGW\mingw64\x86_64-w64-mingw32\include\winddi.h
    on lines 41-44

## Note on types

COM library uses macros to define types with UPPERCASE naming convention. These include some complex types like `IMMDevice`, but also simple types such as `LPCWSTR = w_char_t**`. I use the direct meaning of those types to simplify the explanation. For complete definitions see Microsoft's docs on COM.

When specifying function parameters, I use parentheses `(type)` for types and brackets `[var]` for examples of variable names in use, constants etc. Sometimes only a `[var]` is sufficient for explanation, or the other way around. Just do not expect a complete definition, it's meant only as an explanation.

## `<objbase.h>`

_Provides:_

`__uuidof()`: UUID of coclasses or interfaces

type `CLSID`: Coclass UUID

type `IID`: Interface UUID

type `HRESULT`: Error and success messages. Check with macros:

```C++
HRESULT hr = ...
if (SUCCEEDED(hr)) {}
if (FAILED(hr)) {}
```

To initialize COM library:

```C++
CoInitialize(
    [NULL] // Reserved. Must be NULL.
)
```

To create an instance of a coobject with its interface:

```C++
hr = CoCreateInstance(
    (CLSID),                  // Coclass UUID
    [NULL],                   // Aggregating methods (NULL for no aggregation)
    [CLSCTX_ALL],             // Server type
    (IID),                    // Interface UUID
    (void**) [&ptr_Interface] // Will receive a pointer to the interface
);
```

`ptr_DeviceEnumerator` is of type `(IMMDeviceEnumerator*)`.
`&ptr_DeviceEnumerator` is `(IMMDeviceEnumerator**)`.
CoCreateInstance accepts it as `(void**)`,
which is basically a way to accept any type,
since C doesn't have templates.
It's a pointer to a pointer because `CoCreateInstance()` has to change
the `ptr_Interface` value to point to the corresponding interface.

The previous `CoCreateInstance()` is a shortcut for:

```C++
hr = CoGetClassObject(
    (CLSID),
    [CLSCTX_ALL],
    [NULL],
    [IID_IClassFactory], // ClassFactory interface UUID
    (void**) [&ptr_ClassFactory]
);
hr = pCF->CreateInstance(
    [NULL]                  // Aggregating methods
    (IID)                   // Interface UUID
    (void**) &ptr_Interface // Will receive a pointer to the interface
);
```

To get other interfaces after creating one with `CoCreateInstance()`:

```C++
hr = CoCreateInstance(
    (CLSID),
    [NULL],
    [CLSCTX_ALL],
    (IID),
    (void**) [&ptr_Iterface]
);
hr = ptr_Interface->QueryInterface(
    (IID),                         // Interface UUID
    (void**) [&ptr_OtherInterface] // Will receive a pointer to the interface
);
```

To delete the object:

```C++
ptr_Interface->Release();
```

## Multimedia Device API: `<mmdeviceapi.h>`

_Provides:_

### `IMMDeviceEnumerator`

Provides available audio devices.

To get a list of all available audio devices matching given properties as a `IMMDeviceCollection`:

```C++
::EnumAudioEndpoints(
    (EDataFlow),
        // EDataFlow::eRender for rendering devices.
        // EDataFlow::eCapture for capturing devices.
        // EDataFlow::eAll for all devices.
    (unsigned int),
        // Bitwise OR of one or more of:
        // 0x00000001 DEVICE_STATE_ACTIVE
        // 0x00000002 DEVICE_STATE_DISABLED
        // 0x00000004 DEVICE_STATE_NOTPRESENT
        // 0x00000008 DEVICE_STATE_UNPLUGED
        // 0x0000000F DEVICE_STATEMASK_ALL
    (IMMDeviceCollection**) // Will receive a pointer to the devices collection.
)
```

To get the default audio device as a `IMMDevice`:

```C++
::GetDefaultAudioEndpoint(
    (EDataFlow), // EDataFlow::eRender, ::eCapture
    (ERole),
        // ERole::eConsole for games and system notification sounds.
        // ERole::eMultimedia for music, movies, etc.
        // ERole::eCommunications for voice communications.
    (IMMDevice** )// Will receive a pointer to the device.
)
```

To get a specific device:

First load the `IMMDeviceCollection`, then `IMMDeviceCollection::GetCount()`, iterate them, get the device interface `IMMDevice`, use `IMMDevice::OpenProperyStore()` to get its information, choose one, then `IMMDevice::GetId()`, and pass the ID to:

```C++
::GetDevice(
    (wchar_t**),  // Pointer to a string received from IMMDevice::GetID().
    (IMMDevice**) // Will receive a pointer to the device interface.
)
```

### `IMMDeviceCollection`

Represents a collection of multimedia devices.

As explained above:

```C++
::GetCount(
    (unsigned int*) // Will receive the device count. {n}
)
```

```C++
::Item(
    (unsigned int), // Device number. From 0 to {n} - 1
    (IMMDevice**),  // Will receive a pointer to the device interface.
)
```

### `IMMDevice`

Represents a multimedia device.

As explained above:

```C++
::GetID(
    (char**) // Will receive a pointer to to a string with its ID.
)
```

```C++
::OpenPropertyStore(
    (unsigned int),
        // Storage access mode. Options:
        // STGM_READ
        // STGM_WRITE
        // STGM_READWRITE
    (IPropertyStore**) // Will receive a pointer to the property store interface.
)
```

TODO:

```C++
::Activate(
    (IID&),
    // GUID of the requested interface to be activated. Options:
    // IID_IAudioClient, ...
    [CLSCTX_ALL], // Didn't get the meaning, but se to CLSCTX_ALL.
    [NULL],       // Activation parameters. Set to NULL for IAudioCLient.
    (void**)      // Will receive a pointer to the requested interface.
)
```

## Property System `<propsys.h>`

*Provides:*

### `IPropertyStore`

Provides access to device properties.

To get a value:

```C++
::GetValue(
    (PROPERTYKEY),  // PKEY_Device_FriendlyName etc.
    (PROPVARIANT*)  // Will receive the values.
)
```

The properties are stored in:

### `PROPVARIANT`

To initialize:

```C++
PROPVARIANT device_prop;
PropVariantInit(&device_prop);
```

To clear:

```C++
PropVariantClear(&device_prop);
```

### `PROPERTYKEY`

Property keys to choose which property is requested.

Properties such as `PKEY_Device_FriendlyName`.

For some reason there may be problem with linking the files, where these are defined. A workaround is used in this project to manually define them:

```C++
PROPERTYKEY PKEY_Device_FriendlyName;
GUID IDevice_FriendlyName = { 0xa45c254e, 0xdf1c, 0x4efd, { 0x80, 0x20, 0x67, 0xd1, 0x46, 0xa8, 0x50, 0xe0 } };
PKEY_Device_FriendlyName.pid = 14;
PKEY_Device_FriendlyName.fmtid = IDevice_FriendlyName;
```

## Audio Client `<audioclient.h>`

TODO