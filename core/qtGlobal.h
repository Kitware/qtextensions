/*ckwg +5
 * Copyright 2019 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#ifndef __qtGlobal_h
#define __qtGlobal_h

/// \file

#include <qtExports.h>

#if __cplusplus < 201103L
// Visual studio does not bump their reported standard version until VS 2017
// 15.7 (_MSC_VER == 1914), even though VS 2015 (14.0; _MSC_VER == 1900) has
// "good enough" C++11 support for our purposes.
#  if !(_MSC_VER >= 1900 && _MSC_VER < 1914)
#    error qtExtensions requires C++11 or later
#  endif
#endif

#if __cplusplus >= 201703L
#  define QTE_UNUSED [[mayube_unused]]
#elif defined(__GNUC__)
#  define QTE_UNUSED [[gnu::unused]]
#else
#  define QTE_UNUSED
#endif

/// Annotation for overrides of virtual methods.
/// \showinitializer
///
/// This symbol can be used to annotate overrides of virtual methods. As well
/// as serving as a hint to developers, it will also request compiler
/// verification that a method is being overridden.
///
/// \deprecated
///   This macro predates qtExtensions requiring a C++11 compiler. New code
///   should use \c override directly.
#define QTE_OVERRIDE override

/// Thread-safe constant with opportunistic static storage.
///
/// This macro declares a constant variable, using static storage if it is
/// known that such will be thread safe, and local storage otherwise. This is
/// meant to be used as an opportunistic optimization for constant variables
/// with non-trivial initialization for which a static is desirable, but not
/// required.
///
/// \deprecated
///   Since C++11, initialization of statics is required to be thread safe,
///   but Visual Studio did not implement this until VS 2015. Since we now
///   require at least VS 2015, this macro is no longer needed. New code should
///   use <code>static Type const</code> directly.
///
/// \param T Data type of variable to be declared.
#define QTE_THREADSAFE_STATIC_CONST(T) static T const

/// Declare a private singleton.
///
/// This macro declares a private singleton instance. Unlike a global variable
/// that merely has static duration, the singleton is not created until a
/// request for it is made. Upon such request, the singleton is created in a
/// thread-safe manner; that is, it is guaranteed that the accessor function
/// will return the same instance when called from multiple threads
/// simultaneously. The singleton will be destroyed in an unspecified order
/// when your application or library unloads.
///
/// The singleton accessor function is private to the execution unit in which
/// the singleton is declared.
///
/// \param type
///   The typename of the singleton. The return type of the accessor will be
///   \c type*. Note that \p type must not contain commas; use a typedef if you
///   need to declare a singleton of a template type taking more than one
///   template argument.
/// \param name Name of the accessor function.
///
/// \note You should only use this macro if your singleton needs global scope.
///       Singletons with function-local scope should just declare a static
///       variable instead, which (since C++11) has the same semantics.
///
/// \note Since C++11, it is guaranteed that the singleton will only be
///       constructed once. If multiple threads try to simultaneously access
///       the not-yet-created singleton, one thread will be chosen to complete
///       the initialization while the other threads block.
///
/// \par Example:
/// \code{.cpp}
/// class MySingleton
/// {
/// public:
///     void greet() { qDebug() << "Hello, world!"; }
/// };
/// QTE_PRIVATE_SINGLETON(MySingleton, myself)
///
/// int main()
/// {
///     myself()->greet();
///     return 0;
/// }
/// \endcode
#define QTE_PRIVATE_SINGLETON(type, name) \
    namespace \
    { \
        type* name() \
        { \
            static type theInstance; \
            return &theInstance; \
        } \
    }

/// Declare a private singleton, with arguments.
///
/// This macro is the same as #QTE_PRIVATE_SINGLETON, except that an argument
/// list may also be given. The argument list must be enclosed in parentheses.
///
/// \par Example:
/// \code{.cpp}
/// QTE_PRIVATE_SINGLETON_WITH_ARGS(complex<double>, imaginary, (0.0, 1.0))
/// \endcode
#define QTE_PRIVATE_SINGLETON_WITH_ARGS(type, name, args) \
    namespace \
    { \
        type* name() \
        { \
            static type theInstance args; \
            return &theInstance; \
        } \
    }

/// Declare a class non-copyable.
///
/// This macro is used to make a class non-copyable, by explicitly deleting the
/// copy constructor and assignment operator.
///
/// \param class_name Name of the class in which the macro is used.
///
/// \par Example:
/// \code{.cpp}
/// class Foo
/// {
/// public:
///     ...
/// private:
///     QTE_DISABLE_COPY(Foo)
/// };
/// \endcode
///
/// \note Although the compiler will not generate a default copy constructor
///       and assignment if the base class does not provide public versions of
///       the same, it is recommended that you use this macro for \em all
///       non-copyable classes, even if they are implicitly non-copyable due to
///       a non-copyable base class. Doing so produces better error messages
///       when trying to copy or assign such a class, and makes it more obvious
///       to users that your class is not intended to be copyable.
#define QTE_DISABLE_COPY(class_name) \
    class_name(const class_name&) = delete; \
    class_name& operator=(const class_name&) = delete; \

/// Implement constant d-function with aliased name of private class.
///
/// This is equivalent to #QTE_IMPLEMENT_D_FUNC_CONST, but allows the type name
/// of the private class to be specified with \p private_name, which is
/// necessary if the private class is not named <i>Base</i>Private.
#define QTE_IMPLEMENT_ALIASED_D_FUNC_CONST(public_name, private_name) \
    inline const private_name* public_name::d_func() const \
    { return static_cast<const private_name*>(qGetPtrHelper(this->d_ptr)); }

/// Implement d-function with aliased name of private class.
///
/// This is equivalent to #QTE_IMPLEMENT_D_FUNC, but allows the type name of
/// the private class to be specified with \p private_name, which is necessary
/// if the private class is not named <i>Base</i>Private.
#define QTE_IMPLEMENT_ALIASED_D_FUNC(public_name, private_name) \
    inline private_name* public_name::d_func() \
    { return static_cast<private_name*>(qGetPtrHelper(this->d_ptr)); } \
    QTE_IMPLEMENT_ALIASED_D_FUNC_CONST(public_name, private_name)

/// Implement constant d-function.
///
/// \copydetails #QTE_IMPLEMENT_D_FUNC
/// Unlike QTE_IMPLEMENT_D_FUNC, only the \c const flavor of
/// <code>d_func()</code> is defined. This is useful if your class does not
/// provide a mutable implementation.
#define QTE_IMPLEMENT_D_FUNC_CONST(class_name) \
    QTE_IMPLEMENT_ALIASED_D_FUNC_CONST(class_name, class_name##Private)

/// Implement d-function.
///
/// This implements the d-function (<code>d_func()</code>) of a class using
/// \ref QTE_D.
#define QTE_IMPLEMENT_D_FUNC(class_name) \
    QTE_IMPLEMENT_ALIASED_D_FUNC(class_name, class_name##Private)

/// Implement d-function for implicitly shared class.
///
/// This implements the d-function (<code>d_func()</code>) for
/// \ref qte_d_implicitly_shared using QtExtensions' support for the same.
#define QTE_IMPLEMENT_D_FUNC_SHARED(class_name) \
    inline class_name##Data* class_name::d_func(bool detach) \
    { \
        if (detach) this->d_ptr.detach(); \
        return static_cast<class_name##Data*>(this->d_ptr.data()); \
    } \
    inline const class_name##Data* class_name::d_func() const \
    { return static_cast<const class_name##Data*>(this->d_ptr.constData()); }

/// Declare accessor functions for private class.
///
/// This declares normal (\c const and non-\c const) accessor functions for the
/// private class of \p class_name. The private class is also declared as a
/// friend.
#define QTE_DECLARE_PRIVATE(class_name) \
    inline class_name##Private* d_func(); \
    inline const class_name##Private* d_func() const; \
    friend class class_name##Private;

/// Declare \c const accessor function for private class.
///
/// This declares a \c const accessor function for the private class of
/// \p class_name. The private class is also declared as a friend. No
/// non-\c const accessor is declared.
///
/// Use this when mutable access to the class's is not permitted.
#define QTE_DECLARE_PRIVATE_CONST(class_name) \
    inline const class_name##Private* d_func() const; \
    friend class class_name##Private;

/// Declare accessor functions for implicitly shared class.
///
/// This declares the data class accessor functions for an implicitly shared
/// class \p class_name. The data class is also declared as a friend.
#define QTE_DECLARE_SHARED(class_name) \
    inline class_name##Data* d_func(bool); \
    inline const class_name##Data* d_func() const; \
    friend class class_name##Data;

/// Define accessor functions for public class.
///
/// This declares and defines accessor functions for the public class
/// \p class_name of a corresponding private class. The public class is also
/// declared as a friend.
#define QTE_DECLARE_PUBLIC(class_name) \
    inline class_name* q_func() \
    { return static_cast<class_name*>(q_ptr); } \
    inline const class_name* q_func() const \
    { return static_cast<class_name*>(q_ptr); } \
    friend class class_name;

/// Declare simple pointer to private class.
///
/// This declares a simple pointer (<code>Private* const</code>) to the private
/// class of \p class_name. The private class must be freed when the public
/// class is destroyed to avoid a memory leak.
///
/// In most cases, you should use #QTE_DECLARE_PRIVATE_RPTR instead, to reduce
/// the risk of errors due to the need to perform manual memory management.
#define QTE_DECLARE_PRIVATE_PTR(class_name) \
    class_name##Private* const d_ptr;

/// Declare mutable simple pointer to private class.
///
/// This declares a simple pointer (<code>Private*</code>) to the private class
/// of \p class_name. The private class must be freed when the public class
/// is destroyed to avoid a memory leak.
///
/// Unlike #QTE_DECLARE_PRIVATE_PTR, the pointer can be modified after class
/// construction.
///
/// In most cases, you should use #QTE_DECLARE_PRIVATE_MRPTR instead, to reduce
/// the risk of errors due to the need to perform manual memory management.
#define QTE_DECLARE_PRIVATE_MPTR(class_name) \
    class_name##Private* d_ptr;

/// Declare shared pointer to private class.
///
/// This declares a shared pointer
/// (<code>QSharedPointer\<Private\> const</code>) to the private class of
/// \p class_name. The header for QSharedPointer must be included.
/// QSharedPointer will automatically free the private class when the public
/// class is destroyed.
///
/// This should only be used in special situations; for example, when creating
/// \ref qte_d_shared_explicit.
#define QTE_DECLARE_PRIVATE_SPTR(class_name) \
    QSharedPointer<class_name##Private> d_ptr;

/// Declare scoped pointer to private class.
///
/// This declares a scoped pointer
/// (<code>QScopedPointer\<Private\> const</code>) to the private class of
/// \p class_name. The header for QScopedPointer must be included, and
/// QScopedPointer will automatically free the private class when the public
/// class is destroyed.
///
/// The use of QScopedPointer does not change the semantics of the d-pointer
/// beyond the convenience of ensuring clean-up for you. Therefore, this is
/// preferred over #QTE_DECLARE_PRIVATE_PTR in most cases.
#define QTE_DECLARE_PRIVATE_RPTR(class_name) \
    QScopedPointer<class_name##Private> const d_ptr;

/// Declare mutable scoped pointer to private class.
///
/// This declares a scoped pointer
/// (<code>QScopedPointer\<Private\> const</code>) to the private class of
/// \p class_name. The header for QScopedPointer must be included, and
/// QScopedPointer will automatically free the private class when the public
/// class is destroyed.
///
/// The use of QScopedPointer does not change the semantics of the d-pointer
/// beyond the convenience of ensuring clean-up for you. Therefore, this is
/// preferred over #QTE_DECLARE_PRIVATE_MPTR in most cases.
///
/// Unlike #QTE_DECLARE_PRIVATE_RPTR, the pointer can be modified after class
/// construction.
#define QTE_DECLARE_PRIVATE_MRPTR(class_name) \
    QScopedPointer<class_name##Private> d_ptr;

/// Declare implicitly-managed pointer to data class of implicitly shared class.
///
/// This declares an implicitly managed shared data pointer to the data class
/// of a class that implements \ref implicit-sharing. The pointer is a
/// QSharedDataPointer.
///
/// The data class is automatically copied when requesting a mutable instance
/// (e.g. via #QTE_D_MUTABLE), and freed when its last reference is released.
#define QTE_DECLARE_SHARED_PTR(class_name) \
    QSharedDataPointer<class_name##Data> d_ptr;

/// Declare explicitly-managed pointer to data class of implicitly shared class.
///
/// This declares an explicitly managed shared data pointer to the data class
/// of a class that implements \ref implicit-sharing. The pointer is a
/// QExplicitlySharedDataPointer.
///
/// The data class is freed when its last reference is released.
///
/// Unlike #QTE_DECLARE_SHARED_PTR, copying of the data class is managed
/// explicitly. Retrieving the data class pointer with #QTE_D_MUTABLE will
/// \em not make a copy of the data class, even if the data is shared between
/// multiple objects. Use #QTE_D_DETACH to obtain a non-shared instance of the
/// data class (which will make a copy if necessary).
#define QTE_DECLARE_SHARED_EPTR(class_name) \
    QExplicitlySharedDataPointer<class_name##Data> d_ptr;

/// Declare pointer to public class.
///
/// This declares a simple pointer (<code>Public* const</code>) to the public
/// class \p class_name of a private class.
#define QTE_DECLARE_PUBLIC_PTR(class_name) \
    class_name* const q_ptr;

/// Get pointer to private class.
///
/// This declares a local variable \c d, which is a pointer to the private
/// class of the caller. The pointer itself is immutable (that is, cannot be
/// reassigned).
///
/// \note
///   For historic reasons, this macro accepts optional arguments (which are
///   ignored). The modern version uses \c auto to deduce the type of \c d. New
///   users should omit parameters.
#define QTE_D(...) auto* const d = d_func()

/// Get pointer to private class.
///
/// \deprecated
///   This macro exists for historic reasons (in the bad days when the user had
///   to explicitly specify the class name and whether or not it needed to be
///   \c const qualified). New users should use #QTE_D() (with no argument)
///   instead.
#define QTE_D_CONST(...) QTE_D()

/// Get pointer to shared (immutable) data class.
///
/// This declares a local variable \c d, which is a pointer to the data class
/// of a calling implicitly shared class. Neither the pointer nor the data
/// class may be modified. The referenced data class may be shared with other
/// instances.
///
/// \note
///   For historic reasons, this macro accepts optional arguments (which are
///   ignored). The modern version uses \c auto to deduce the type of \c d. New
///   users should omit parameters.
#define QTE_D_SHARED(...) const auto* const d = d_func()

/// Get pointer to mutable data class.
///
/// This declares a local variable \c d, which is a pointer to the data class
/// of a calling implicitly shared class. The pointer allows the data class to
/// be modified, but is itself immutable (that is, cannot be reassigned).
///
/// Before returning, the data pointer is explicitly detached; that is, if it
/// was shared with any other instances, a copy is made to ensure that the
/// returned pointer is not shared. This is intended to be used by classes that
/// use explicitly managed data pointers (i.e. declared with
/// #QTE_DECLARE_SHARED_EPTR). For implicitly managed data pointers, it is
/// functionally equivalent to #QTE_D_MUTABLE, but may be marginally less
/// efficient.
///
/// \note
///   For historic reasons, this macro accepts optional arguments (which are
///   ignored). The modern version uses \c auto to deduce the type of \c d. New
///   users should omit parameters.
#define QTE_D_DETACH(...) auto* const d = d_func(true)

/// Get pointer to mutable data class.
///
/// This declares a local variable \c d, which is a pointer to the data class
/// of a calling implicitly shared class. The pointer allows the data class to
/// be modified, but is itself immutable (that is, cannot be reassigned).
///
/// If the data class pointer is implicitly managed (i.e. was declared with
/// #QTE_DECLARE_SHARED_PTR), the referenced data class is detached; that is,
/// it is guaranteed that no other instances refer to the same data class (a
/// copy of the data class is made if necessary to ensure this). If the pointer
/// is explicitly managed, it is possible that the data class is shared with
/// other instances. Use #QTE_D_DETACH when dealing with an explicitly managed
/// data pointer when a non-shared data instance is required.
///
/// \note
///   For historic reasons, this macro accepts optional arguments (which are
///   ignored). The modern version uses \c auto to deduce the type of \c d. New
///   users should omit parameters.
#define QTE_D_MUTABLE(...) auto* const d = d_func(false)

/// Get pointer to public class.
///
/// This declares a local variable \c q, which is a pointer to the public class
/// of the caller. The pointer itself is immutable (that is, cannot be
/// reassigned).
///
/// \note
///   For historic reasons, this macro accepts optional arguments (which are
///   ignored). The modern version uses \c auto to deduce the type of \c q. New
///   users should omit parameters.
#define QTE_Q(...) auto* const q = q_func()

/// Get pointer to public class.
///
/// \deprecated
///   This macro exists for historic reasons (in the bad days when the user had
///   to explicitly specify the class name and whether or not it needed to be
///   \c const qualified). New users should use #QTE_D() (with no argument)
///   instead.
#define QTE_Q_CONST(...) QTE_Q()

/// Alias for #foreach_child.
///
/// This macro is a synonym for #foreach_child that is guaranteed to be
/// available. (If QT_NO_KEYWORDS is defined, the more convenient
/// #foreach_child is hidden to avoid namespace pollution.)
#define QTE_FOREACH_CHILD(variable, parent) \
    for (int _ii = 0, _ij = 0, _ik = (parent)->childCount(); \
         _ii == _ij && _ii < _ik; ++_ij) \
        for (variable = (parent)->child(_ii); _ii == _ij; ++_ii)

/// Alias for #foreach.
///
/// This macro is a synonym for #foreach that is guaranteed to be available.
/// (If QT_NO_KEYWORDS is defined, the more convenient #foreach is hidden to
/// avoid namespace pollution.)
#  define QTE_FOREACH(decl, container) for (decl : container)

/// Alias for #foreach_iter.
///
/// This macro is a synonym for #foreach_iter that is guaranteed to be
/// available. (If QT_NO_KEYWORDS is defined, the more convenient
/// #foreach_iter is hidden to avoid namespace pollution.)
#define QTE_FOREACH_ITER(iterator_type, variable, container) \
    for (iterator_type variable = (container).begin(), \
         _end = (container).end(); \
         variable != _end; ++variable)

#if __cplusplus >= 201703L || defined(DOXYGEN)
/// Alias for #using.
///
/// This macro is a synonym for #using that is guaranteed to be available.
/// (If QT_NO_KEYWORDS is defined, the more convenient #using is hidden to
/// avoid namespace pollution.)
#  define QTE_USING(...) if(__VA_ARGS__; true)
#else
#  define QTE_USING(...) \
    QTE_WITH_IMPL(_qte_scope_flag ## __LINE__, __VA_ARGS__)

#  define QTE_WITH_IMPL(guard, ...) \
    for (int guard = 0; !guard;) \
        for (__VA_ARGS__; !guard; ++guard)
#endif

/// Alias for #with.
///
/// This macro is a synonym for #with that is guaranteed to be available.
/// (If QT_NO_KEYWORDS is defined, the more convenient #with is hidden to
/// avoid namespace pollution.)
#define QTE_WITH(...) \
    QTE_USING(auto&& _with ## __LINE__ QTE_UNUSED = __VA_ARGS__)

/// Alias for #synchronized.
///
/// This macro is a synonym for #synchronized that is guaranteed to be
/// available. (If QT_NO_KEYWORDS is defined, the more convenient #synchronized
/// is hidden to avoid namespace pollution.)
#define QTE_SYNCHRONIZED(mutex) QTE_WITH(QMutexLocker{mutex})

#ifdef DOXYGEN
/// Iterate over children.
///
/// This macro iterates over the children of any class that implements the
/// methods <code>int childCount()</code> and <code>child(int)</code>; e.g.
/// QTreeWidgetItem.
///
/// \param variable
///   Name of variable that receives the child. The type of \p variable must
///   be assignable from the return of <code>parent.child()</code>.
/// \param parent Parent of children over which to iterate.
///
/// \par Example:
/// \code{.cpp}
/// // Iterate over each top-level item in 'tree'
/// QTreeWidget* tree;
/// foreach_child (QTreeWidgetItem* item, tree->invisibleRootItem())
///     processItem(item);
/// \endcode
///
/// \deprecated
///   New code should use range-based \c for with #qtChildren instead.
#  define foreach_child(variable, parent)
/// Iterate over container.
///
/// This macro provides for simple iteration over the elements in a container.
/// It is an alias for a
/// <a href="http://en.cppreference.com/w/cpp/language/range-for">range-based
/// for</a> loop.
///
/// \note Unlike %Qt's \c foreach, QtExtensions' \c foreach does \em not make a
///       copy of \p container. You should not modify \p container (i.e. by
///       adding or removing items) within the loop. It is safe to modify items
///       in place.
///
/// \par Example 1:
/// \code{.cpp}
/// // QStringList getStrings();
/// foreach(auto const& s, getStrings())
///     qDebug() << s;
/// \endcode
///
/// \par Example 2:
/// \code{.cpp}
/// // QStringList strings;
/// foreach(auto& s, strings)
///     s = s.toLower();
/// \endcode
///
/// \deprecated New code should use range-based \c for instead.
#  define foreach(declaration, container)
/// Iterate over a copy of a container.
///
/// This macro provides for simple iteration over the elements in a container,
/// with the additional safety that a copy of the container is used for
/// iteration. This allows safely making changes (such as insertions or
/// removals) to the original container during iteration. Such changes will not
/// be visible to iteration.
///
/// This is implemented using %Qt's \c foreach.
#  define foreach_copy(declaration, container) Q_FOREACH
/// Iterate over container using an iterator.
///
/// This macro simplifies iterating over a container using an iterator. This
/// has the advantage over %Qt's \c foreach in that it works also with STL
/// containers (without making an expensive copy), and can be used on Qt
/// associative containers (e.g. QMap, QHash) when knowing the key is required,
/// and iterating over the key set is undesirable either to avoid the extra
/// cost of value look-up, or the inconvenience of dealing with maps with
/// non-unique keys (e.g. QMultiMap).
///
/// \param iterator_type
///   The typename of the iterator object; typically \c auto. Note that
///   \p iterator_type must not contain commas.
/// \param variable Name of the iterator variable.
/// \param container
///   Container over which to iterate. This should be a constant expression
///   (usually a variable name), as it is evaluated more than once.
///
/// \note Unlike %Qt's \c foreach, \c foreach_iter does \em not make a copy of
///       \p container. You should not modify \p container (i.e. by adding or
///       removing items) within the loop. It is safe to modify items in place.
///
/// \par Example:
/// \code{.cpp}
/// typedef QHash<int, QString> MyMap;
/// MyMap map = createMap();
/// foreach_iter(MyMap::const_iterator, iter, map)
///     qDebug() << "key" << iter.key() << "value" << iter.value();
/// \endcode
///
/// \deprecated
///   New code should use range-based \c for with #qtEnumerate or
///   #qtEnumerateMutable instead.
#  define foreach_iter(iterator_type, variable, container)
/// Use a declaration in a scope.
///
/// This macro creates a scope within which a declaration is active. If the
/// code section is a single statement, the use of braces around the code
/// section is optional.
///
/// \param decl Declaration statement that will be active within the scope.
///
/// \par Example:
/// \code{.cpp}
/// using(QFile f)
/// {
///     f.open(path);
///     ...
/// }
/// \endcode
#  define using(decl)
/// Use an expression in a scope.
///
/// This macro creates a scope within which an expression is active. This is
/// useful for RAII types where management of the object's lifetime is
/// critical, but the object itself does not need to be accessed. If the code
/// section is a single statement, the use of braces around the code section is
/// optional.
///
/// \param expr Assignable expression that will be active within the scope.
///
/// \par Example:
/// \code{.cpp}
/// with(qtScopedSettingsGroup{s, "group"})
/// {
///     ...
/// }
/// \endcode
///
/// \sa #synchronized
#  define with(expr)
/// Declare a critical code section.
///
/// This macro declares a critical section of code that is protected by the
/// named mutex. It is similar to the Java keyword, except that only a QMutex
/// may be used as the lock object. If the code section is a single statement,
/// the use of braces around the code section is optional.
///
/// \param mutex Pointer to a QMutex used to synchronize execution of the
///              critical section.
#  define synchronized(mutex)
#else
#  ifndef QT_NO_KEYWORDS
#    if defined(foreach) && defined(Q_FOREACH)
#      undef foreach
#    endif
#    ifndef foreach
#      define foreach QTE_FOREACH
#    endif
#    ifndef foreach_copy
#      define foreach_copy Q_FOREACH
#    endif
#    ifndef foreach_child
#      define foreach_child QTE_FOREACH_CHILD
#    endif
#    ifndef foreach_iter
#      define foreach_iter QTE_FOREACH_ITER
#    endif
#    ifndef using
#      define using(...) QTE_USING(__VA_ARGS__)
#    endif
#    ifndef with
#      define with QTE_WITH
#    endif
#    ifndef synchronized
#      define synchronized QTE_SYNCHRONIZED
#    endif
#  endif
#endif

/// Shorthand to register a metatype with %Qt.
///
/// This macro provides a convenient shorthand to register a %Qt metatype
/// using qRegisterMetaType. It passes the stringified type \p type as the
/// textual name, saving the users having to type the name twice.
#define QTE_REGISTER_METATYPE(type) qRegisterMetaType<type>(#type)

#endif
