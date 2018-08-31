#pragma once

/* Local libraries: */
#include "api/wasapi/interface.hpp"

/* Windows libraries: */
// Multimedia Device API:
#include <mmdeviceapi.h>

/* Standard libraries: */
#include <string>

namespace Cynth::API::WASAPI {

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

}