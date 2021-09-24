
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */






/*
* Classification
*  
* UNCLASSIFIED
*
*/    
#ifndef FieldIterator_h
#define FieldIterator_h

// managed
#include "Field_Net.h"
#include "GMSEC_Net.h"

// native
#include "gmsec/FieldIterator.h"

namespace Nasa
{
namespace Gmsec
{
namespace Net
{

/// <summary>
/// Choose what type of fields to iterate over
/// </summary>
public enum class FieldSelectorChoice
{
	/// <summary>
	/// All Fields
	/// </summary>
	SelectAllFields, 
	
	/// <summary>
	/// All Header Fields
	/// </summary>
	SelectHeaderFields, 
	
	/// <summary>
	/// All Non Header Fields
	/// </summary>
	SelectNonheaderFields
};

/// <summary>
/// The FieldIterator supports iterating over the Fields of a Message.
/// FieldIterators are non-intrusive.  Since they are non-intrusive there can
/// be more than one iterating over a single Message at the same time.
/// This differs from Message GetFirstElement/GetNextElement.  It is allowed
/// to use both GetFirstElement/GetNextElement and FieldIterator(s) on a
/// Message.
/// After constructing a FieldIterator, iteration is initiated by invoking
/// Message::Reset(FieldIterator^ iterator).  For example,
/// </summary>
/// <remarks>
/// Note that modifying the Fields of a Message (e.g., by calling AddField,
/// ClearFields, or ClearFields) while iterating over it is not supported.
/// Further note that FieldIterators are not thread safe.
/// </remarks>
/// <example>
/// <code lang="C++">
/// Status^ applyToHeaderFields (Message^ message)
/// {
/// FieldIterator^ i = gcnew FieldIterator(FieldSelectorChoice::selectHeaderFields);
/// Field^ field = gcnew Field();
/// Status^ result = message->Reset(i);
/// while (!result->IsError() &amp;&amp; i->HasNext())
/// {
/// 	result = i->Next(field);
/// 	if (!result->IsError())
/// 	{
/// 		// do something on field
/// 	}
/// }
/// return result;
/// }
/// </code>
/// <code lang="C#">
/// Status applyToHeaderFields (Message message)
/// {
/// FieldIterator i = new FieldIterator(FieldSelectorChoice.selectHeaderFields);
/// Field field = new Field();
/// Status result = message.Reset(i);
/// while (!result.IsError() &amp;&amp; i.HasNext())
/// {
/// 	result = i.Next(field);
/// 	if (!result.IsError())
/// 	{
/// 		// do something on field
/// 	}
/// }
/// return result;
/// }
/// </code>
/// <code lang="VB.NET">
/// Function applyToHeaderFields(message As Message) As Status
/// Dim i As New FieldIterator(FieldSelectorChoice.selectHeaderFields)
/// Dim field As New Field()
/// Dim result As Status
/// result = message.Reset(i)
/// While ((Not result.IsError()) And i.HasNext())
///     result = i.Next(field)
///     If (Not result.IsError()) Then
///         ' do something on field
///     End If
/// End While
/// Return result
/// End Function
/// </code>
/// <code lang="f#">
/// F#
/// let applyToHeaderFields (message : Message) = 
///     let i = new FieldIterator(FieldSelectorChoice.selectHeaderFields);
///     let field = new Field();
///     let result = message.Reset(i);
///     let IsError = result.IsError()
///     let hasNext = i.HasNext()
///     while not IsError &amp;&amp; hasNext do
///         let result3 = i.Next(field)
///         if( result3.IsError() ) then () // Handle Error
/// </code>
/// </example>
[Obsolete("API 3.x has been deprecated; use API 4.x instead.")]
public ref class FieldIterator
{
private:
	gmsec::FieldIterator * mImpl;

internal:
	/// <summary>Get unmanaged implementation version</summary>
    /// <returns>Unmanaged implementation</returns>
	gmsec::FieldIterator * GetUnmanImpl();
	
	/// <summary>Reset unmanaged implementation to null</summary>
	void ResetUnmanImpl();

	/// <summary>Delete old impl if present, and set unmanaged implementation to new value</summary>
	void SetUnmanImpl(gmsec::FieldIterator* fieldIterator);

protected:
	/// <summary>Finalizer, free memory and set objects to null</summary>
	!FieldIterator();

public:
	/// <summary>Initialize a FieldIterator without a selector.</summary>
	FieldIterator();
	
	/// <summary>Initialize a FieldIterator with a selector.
	/// Only Fields which the selector function returns true for will be returned by the iterator.</summary>
	/// <param name="fieldSelector">Field selector function pointer, only for use by C++ CLI</param>
	FieldIterator(gmsec::FieldSelector fieldSelector);
	
	/// <summary>Initialize a FieldIterator with a predefined selector choice</summary>
	/// <param name="fieldSelectorChoice">enum of type of fields wanted iterated over</param>
	FieldIterator(Nasa::Gmsec::Net::FieldSelectorChoice fieldSelectorChoice);
	
	/// <summary>Destructor - cleans up the FieldIterator instance</summary>
	~FieldIterator();
	
	/// <summary>Indicates whether a Next call may be made</summary>
	/// <returns>Returns true if there is a Next field.</returns>
	bool HasNext();

	/// <summary>Retrieves the next Field in the iteration.
	/// If Next is called when there is no field to be read, an error is
	/// returned with class GMSECErrorClasses::STATUS_ITERATOR_ERROR and code
	/// GMSECErrorCodes::INVALID_NEXT.</summary>
	/// <param name="field">Reference through which the next Field is returned.</param>
	/// <returns>Returns a Status object indicating the success or failure of this call.</returns>
	Status^ Next(Field^ field);
};

}
}
}

#endif  // FieldIterator_h
