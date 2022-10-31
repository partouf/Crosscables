
#ifndef __GFProperty_H
#define __GFProperty_H

#include "../Atoms/Freeable.h"
#include "../Molecules/Callbacks.h"
#include "../Molecules/String.h"

/** BasicProperty, don't use this base class.
  *  Properties are object containers with functions to get/set the object (or object-pointer).
  *  The internalSet() and internalGet() functions are public to serve the implementor functions of the
  *   get() and set() functions that may be customized by non-inheriting classes.
  **/
namespace Groundfloor {
	template <class T>
	class BasicProperty : public Freeable
	{
	protected:
		T anObject;
	public:
		/// initializes internal object to 0
		BasicProperty() : Freeable::Freeable() {
			this->anObject = 0;
		}
		~BasicProperty() {
		}

		/// the default SET function (invokes internalSet())
		virtual void set(T object) {
			internalSet(object);
		}
		/// the default GET function (invokes internalGet())
		virtual T get() const {
			return internalGet();
		}

		/// sets the internal object to given object
		void internalSet(T object) {
			this->anObject = object;
		}
		/// returns the internal object
		T internalGet() const {
			return this->anObject;
		}
	};

	/// Property is the default property class to use,
	///  use it in the public section of your classes
	///  defined by for example: Property<int> myInteger;
	///  or Property<TMyObject *> myObject;
	template <class T>
	class Property : public BasicProperty<T>
	{
	protected:
		NotifyAbstract<T> *aNotify;
		RetreiveAbstract<T> *aRetreive;

		bool bAutoFreeNotify;
		bool bAutoFreeRetreive;
	public:
		/// initializes property, sets notify/retreive functions to NULL
		Property();
		/// finalizes property and deletes notify/retreive functions if AutoFree is True
		~Property();

		/// sets the notifier function that is invoked when the SET function is called
		/// notifier must conform to format [void yourfunction( <T> data )]
		/** EXAMPLE: property.setNotify( GFCreateNotify( TMyObject *, TMyClass, this, &TMyClass::myFunction ) );
		 */
		void setNotify(NotifyAbstract<T> *anEventHandler, bool bAutoFree = true);
		/// sets the retreive function that is invoked when the GET function is called
		void setRetreive(RetreiveAbstract<T> *anEventHandler, bool bAutoFree = true);

		/// implemenation SET function: calls Notify function if set, otherwise it invokes internalSet() directly
		void set(T object);
		/// implemenation GET function: calls Retreive function if set, otherwise it invokes internalGet() directly
		T get() const;
	};

	/// special class for string-properties, don't use this base class
	class BasicStringProperty : public Freeable
	{
	protected:
		String anObject;
	public:
		/// initializes internal string to ""
		BasicStringProperty();
		~BasicStringProperty();

		/// Returns a pointer to the String that is used.
		/// Do not call this function unless you really need it due to optimisation for example.
		String *link(bool bDoRetreive = true);

		/// invokes
		virtual void set(const char *object);
		virtual void set(const String *object);
		virtual char *get() const;

		unsigned int getLength();

		void internalSetCopy(const String *object);
		void internalSet(const char *object);
		char *internalGet() const;
	};

	/// String-property-class.
	/// The reason why there is a seperate property-class for strings, is because changes to the actual string
	///  cannot be tracked when a normal [Property<String *> myProperty] is used.
	/// By forcing the developer to use char * set- and get-values, we know exactly when the internal string is being changed.
	class StringProperty : public BasicStringProperty
	{
	protected:
		NotifyAbstract<const char *> *aNotify;
		RetreiveAbstract<char *> *aRetreive;

		bool bAutoFreeNotify;
		bool bAutoFreeRetreive;
	public:
		/// sets internal string to "" and sets notify/retreive functions to NULL
		StringProperty();
		~StringProperty();

		/// sets the notify function, must conform to [void myFunction( const char *myText )]
		void setNotify(NotifyAbstract<const char *> *anEventHandler, bool bAutoFree = true);
		/// sets the retreive function, must conform to [char *myFunction()]
		void setRetreive(RetreiveAbstract<char *> *anEventHandler, bool bAutoFree = true);

		/// invokes notify function if available, otherwise internalSet() is invoked directly
		void set(const char * object);
		void set(const String *object);
		/// invokes retreive function if available, otherwise internalGet() is invoked directly
		char *get() const;

		String *link(bool bDoRetreive = true);
	};

	/// base class for widechar strings, do not use this base class
	class BasicWideStringProperty : public Freeable
	{
	protected:
		String anObject;
	public:
		BasicWideStringProperty();
		~BasicWideStringProperty();

		String *link();

		virtual void set(const wchar_t *object);
		virtual void set(const String *object);
		virtual wchar_t *get() const;

		unsigned int getLength();

		virtual void internalSet(const wchar_t *object);
		virtual void internalSetCopy(const String *object);
		wchar_t *internalGet() const;
	};

	/// Special property class for Widestrings, same as StringProperty, except it forces the use of wchar_t * values.
	class WideStringProperty : public BasicWideStringProperty
	{
	protected:
		NotifyAbstract<const wchar_t *> *aNotify;
		RetreiveAbstract<wchar_t *> *aRetreive;

		bool bAutoFreeNotify;
		bool bAutoFreeRetreive;
	public:
		WideStringProperty();
		~WideStringProperty();

		void setNotify(NotifyAbstract<const wchar_t *> *anEventHandler, bool bAutoFree = true);
		void setRetreive(RetreiveAbstract<wchar_t *> *anEventHandler, bool bAutoFree = true);

		void set(const wchar_t * object);
		void set(const String *object);
		wchar_t *get() const;
	};


	//----------------------------------------------------------------------------



	template <class T>Property<T>::Property() : BasicProperty<T>() {
		this->aNotify = 0;
		this->aRetreive = 0;
	}

	template <class T>Property<T>::~Property() {
		if ((this->aNotify != 0) && (this->bAutoFreeNotify)) {
			delete this->aNotify;
		}

		if ((this->aRetreive != 0) && (this->bAutoFreeRetreive)) {
			delete this->aRetreive;
		}
	}

	template <class T>void Property<T>::setNotify(NotifyAbstract<T> *anEventHandler, bool bAutoFree) {
		if ((this->aNotify != 0) && (this->bAutoFreeNotify)) {
			delete this->aNotify;
		}

		this->aNotify = anEventHandler;
		this->bAutoFreeNotify = bAutoFree;
	}

	template <class T>void Property<T>::setRetreive(RetreiveAbstract<T> *anEventHandler, bool bAutoFree) {
		if ((this->aRetreive != 0) && (this->bAutoFreeRetreive)) {
			delete this->aRetreive;
		}

		this->aRetreive = anEventHandler;
		this->bAutoFreeRetreive = bAutoFree;
	}

	template <class T>void Property<T>::set(T object) {
		if (this->aNotify != 0) {
			this->aNotify->execute(object);
		}
		else {
			this->internalSet(object);
		}
	}

	template <class T>T Property<T>::get() const {
		if (this->aRetreive != 0) {
			return this->aRetreive->retreive();
		}
		else {
			return this->internalGet();
		}
	}
}

//----------------------------------------------------------------------------

#endif // __GFProperty_H

