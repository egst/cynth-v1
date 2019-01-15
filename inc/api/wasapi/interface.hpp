#pragma once

namespace Cynth::API::WASAPI {
    /*/ Generic interface:
    
    Provides a COM interface encapsulation.

    `Interface` takes a COM interface type as a template parameter.
    It then serves as a wrapper for a pointer to an instance of this interface.

    When creating a new derived `Interface`, add the correstponding type
    to the "Template instances" section of `interface.cpp`.
    /*/
    template<typename interface_t>
    class Interface {
      public:
        /*/ Automatic release:

        The underlying interface instance is released upon destruction
        by calling its `->Release()` method.

        The destructor is declared virtual so that it may be modified
        for a derived `Interface` to handle cleanup of a more specific interface.
        
        To allow or suppress the automatic release upon destruction,
        the `::autoRelease()` and `::noAutoRelease()` methods are used.
        These serve only as mutators for the `::auto_release` property
        which is then checked by the destructor
        so there is no need to override them.

        `auto_release` is set to `true` upon construction by default
        and to `false` when the `Interface` is returned by another method.
        Don't forget to use the `::autoRelease()` method after receiving
        such an `Interface` if automatic release is needed.
        /*/
        virtual ~Interface();
        void autoRelease();   // Allow auto release. (default)
        void noAutoRelease(); // Suppress auto release.

        /*/ Move & copy:

        Copy constructors throw an exception.
        This way `Interface` can never be copied, only moved.
        Derived `Interfce`s may override the copy constructors to notify
        the end user of which `Interface` was to be moved.
        TODO: Is it possible to handle this in the base class?

        Move constructors are defined as default - member-wise move.
        /*/
        Interface(
            const Interface<interface_t>& other);          // Copy constructor

        virtual Interface<interface_t>& operator=(
            const Interface<interface_t>& other);          // Copy assignment

        Interface(Interface<interface_t>&& other); // Move constructor

        virtual Interface<interface_t>& operator=(
            Interface<interface_t>&& other);               // Move assignment
    
      protected:
        /*/ Underlying interface access:

        Methods of the underlying interface may be accessed directly
        by the overloaded `::operator->()`.
        However, wrappers for needed methods should be provided
        by a derived `Interface` to avoid such direct use for the end user.
        When a derived `Interface` is under construction in a testing state
        it may declare this accessor as public explcitly.
        These wrappers' implementation should match those criteria:
        * Avoiding in and out parameters.
        * Sparing the caller from use of pointers.
        * Handling HRESULTS and throwing exceptions when impossible to handle.
        * In case of returning another `Interface`:
            Return by value and handle auto release.
        * Naming:
            * According to the local naming convention - camelCase.
            Appart from the casing:
            * If it only wraps a COM method, it should be named the same.
                (Exceptions may occur if it matches the local structure.)
            * If it provides a more specific or more complex functionality,
                it should use the keywords from the underlying COM methods.
        /*/
        interface_t* operator->();

        /*/ Construction:

        The pointer to the underlying interface instance is set
        upon construction and nothing else is done by the base constructor.

        Some derived `Interface`s are meant be constructed explicitly
        by the end user. These should provide a public constructor explicitly.
        Others are only meant to be constructed and returned by a method
        of another `Interface`. In these cases, the constructor should
        remain private and the `Interface`s authorized to create them
        should be declared as friends.
        // TODO: Can this cause problems when the basic ctor is called implicitly?
        The default costructor should not be accessible to the end user.

        See the "Automatic release" section for details
        on the `auto_release` value in different scenarios of construction.

        Constructors should match the criteria listed in the section above
        except for the use of pointers when private.
        /*/
      public:
		Interface(interface_t* ptr_instance);
		
      protected:
		Interface();

        // Pointer to the underlying interface instance:
        interface_t* ptr_instance;
        // TODO: Switch to safe pointers?
        
        // Automatic release state:
        bool auto_release;
    };
}