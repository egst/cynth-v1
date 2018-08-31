/* An implementation file for: */
#include "api/wasapi/property_store.hpp"

/* Local libraries: */
#include "logger.hpp"
#include "tools.hpp"

using namespace Cynth::API::WASAPI;
using Cynth::Logger;
using Cynth::Tools::wcharToString;

PropertyStore::~PropertyStore() {
    PropVariantClear(&this->property.prop_variant);
}

PropertyStore::PropertyStore(): Interface<IPropertyStore>::Interface() {}

PropertyStore::PropertyStore(IPropertyStore* ptr_instance)
    : Interface<IPropertyStore>::Interface(ptr_instance) {}

PropertyStore::PropertyStore(const PropertyStore& other)
    : Interface<IPropertyStore>::Interface(other) {}

PropertyStore::PropertyStore(PropertyStore&& other) = default;

PropertyStore& PropertyStore::operator=(const PropertyStore& other) {
    Interface<IPropertyStore>::operator=(other);
    return *this;
}

PropertyStore& PropertyStore::operator=(PropertyStore&& other) = default;

PropertyStore::prop_t& PropertyStore::getValue(PROPERTYKEY property_key) {
    PropVariantClear(&this->property.prop_variant);
    PropVariantInit(&this->property.prop_variant);
    HRESULT hr = this->ptr_instance->GetValue(
        property_key,
        &this->property.prop_variant);
    if (FAILED(hr))
        Logger::errHRESULT(hr);
    this->property.str_val = wcharToString(this->property.prop_variant.pwszVal);
    return this->property;
}