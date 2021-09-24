/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

/* This file is used for generating Doxygen documentation; it only contains
   stubs of constructors and methods associated with the class.
*/


namespace GMSEC.API {

/// <summary>Class that is used to manage a collection of MessageSpecification objects.</summary>

public class MessageSpecificationList : System.IDisposable, System.Collections.IEnumerable {

  public MessageSpecificationList(System.Collections.ICollection c) : this() {
    if (c == null)
      throw new System.ArgumentNullException("c");
    foreach (MessageSpecification element in c) {
      this.Add(element);
    }
  }

  public bool IsFixedSize {
    get {
      return false;
    }
  }

  public bool IsReadOnly {
    get {
      return false;
    }
  }

  public MessageSpecification this[int index]  {
    get {
      return getitem(index);
    }
  }

  public int Count {
    get {
      return (int)size();
    }
  }

  public bool IsSynchronized {
    get {
      return false;
    }
  }

  public void CopyTo(System.Array array) {
    CopyTo(0, array, 0, this.Count);
  }

  public void CopyTo(System.Array array, int arrayIndex) {
    CopyTo(0, array, arrayIndex, this.Count);
  }

  public void CopyTo(int index, System.Array array, int arrayIndex, int count) {
    if (array == null)
      throw new System.ArgumentNullException("array");
    if (index < 0)
      throw new System.ArgumentOutOfRangeException("index", "Value is less than zero");
    if (arrayIndex < 0)
      throw new System.ArgumentOutOfRangeException("arrayIndex", "Value is less than zero");
    if (count < 0)
      throw new System.ArgumentOutOfRangeException("count", "Value is less than zero");
    if (array.Rank > 1)
      throw new System.ArgumentException("Multi dimensional array.");
    if (index+count > this.Count || arrayIndex+count > array.Length)
      throw new System.ArgumentException("Number of elements to copy is too large.");
    for (int i=0; i<count; i++)
      array.SetValue(getitemcopy(index+i), arrayIndex+i);
  }

  // Type-safe version of IEnumerable.GetEnumerator
  System.Collections.IEnumerator System.Collections.IEnumerable.GetEnumerator() {
    return new MessageSpecificationListEnumerator(this);
  }

  public MessageSpecificationListEnumerator GetEnumerator() {
    return new MessageSpecificationListEnumerator(this);
  }

  // Type-safe enumerator
  /// Note that the IEnumerator documentation requires an InvalidOperationException to be thrown
  /// whenever the collection is modified. This has been done for changes in the size of the
  /// collection but not when one of the elements of the collection is modified as it is a bit
  /// tricky to detect unmanaged code that modifies the collection under our feet.
  public sealed class MessageSpecificationListEnumerator : System.Collections.IEnumerator {
    private MessageSpecificationList collectionRef;
    private int currentIndex;
    private object currentObject;
    private int currentSize;

    public MessageSpecificationListEnumerator(MessageSpecificationList collection) {
      collectionRef = collection;
      currentIndex = -1;
      currentObject = null;
      currentSize = collectionRef.Count;
    }

    // Type-safe iterator Current
    public MessageSpecification Current {
      get {
        if (currentIndex == -1)
          throw new System.InvalidOperationException("Enumeration not started.");
        if (currentIndex > currentSize - 1)
          throw new System.InvalidOperationException("Enumeration finished.");
        if (currentObject == null)
          throw new System.InvalidOperationException("Collection modified.");
        return (MessageSpecification)currentObject;
      }
    }

    // Type-unsafe IEnumerator.Current
    object System.Collections.IEnumerator.Current {
      get {
        return Current;
      }
    }

    public bool MoveNext() {
      int size = collectionRef.Count;
      bool moveOkay = (currentIndex+1 < size) && (size == currentSize);
      if (moveOkay) {
        currentIndex++;
        currentObject = collectionRef[currentIndex];
      } else {
        currentObject = null;
      }
      return moveOkay;
    }

    public void Reset() {
      currentIndex = -1;
      currentObject = null;
      if (collectionRef.Count != currentSize) {
        throw new System.InvalidOperationException("Collection modified.");
      }
    }
  }

  public void Clear() {
    GmsecPINVOKE.MessageSpecificationList_Clear(swigCPtr);
    if (GmsecPINVOKE.SWIGPendingException.Pending) throw GmsecPINVOKE.SWIGPendingException.Retrieve();
  }

  public void Add(MessageSpecification value) {
    GmsecPINVOKE.MessageSpecificationList_Add(swigCPtr, MessageSpecification.getCPtr(value));
    if (GmsecPINVOKE.SWIGPendingException.Pending) throw GmsecPINVOKE.SWIGPendingException.Retrieve();
  }

  private int size() {
    int ret = GmsecPINVOKE.MessageSpecificationList_size(swigCPtr);
    if (GmsecPINVOKE.SWIGPendingException.Pending) throw GmsecPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public MessageSpecificationList() : this(GmsecPINVOKE.new_MessageSpecificationList(), true) {
    if (GmsecPINVOKE.SWIGPendingException.Pending) throw GmsecPINVOKE.SWIGPendingException.Retrieve();
  }

  private MessageSpecification getitemcopy(int index) {
    global::System.IntPtr cPtr = GmsecPINVOKE.MessageSpecificationList_getitemcopy(swigCPtr, index);
    MessageSpecification ret = (cPtr == global::System.IntPtr.Zero) ? null : new MessageSpecification(cPtr, false);
    if (GmsecPINVOKE.SWIGPendingException.Pending) throw GmsecPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  private MessageSpecification getitem(int index) {
    global::System.IntPtr cPtr = GmsecPINVOKE.MessageSpecificationList_getitem(swigCPtr, index);
    MessageSpecification ret = (cPtr == global::System.IntPtr.Zero) ? null : new MessageSpecification(cPtr, false);
    if (GmsecPINVOKE.SWIGPendingException.Pending) throw GmsecPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public void AddRange(MessageSpecificationList values) {
    GmsecPINVOKE.MessageSpecificationList_AddRange(swigCPtr, MessageSpecificationList.getCPtr(values));
    if (GmsecPINVOKE.SWIGPendingException.Pending) throw GmsecPINVOKE.SWIGPendingException.Retrieve();
  }

  public MessageSpecificationList GetRange(int index, int count) {
    global::System.IntPtr cPtr = GmsecPINVOKE.MessageSpecificationList_GetRange(swigCPtr, index, count);
    MessageSpecificationList ret = (cPtr == global::System.IntPtr.Zero) ? null : new MessageSpecificationList(cPtr, true);
    if (GmsecPINVOKE.SWIGPendingException.Pending) throw GmsecPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public void Insert(int index, MessageSpecification value) {
    GmsecPINVOKE.MessageSpecificationList_Insert(swigCPtr, index, MessageSpecification.getCPtr(value));
    if (GmsecPINVOKE.SWIGPendingException.Pending) throw GmsecPINVOKE.SWIGPendingException.Retrieve();
  }

  public void InsertRange(int index, MessageSpecificationList values) {
    GmsecPINVOKE.MessageSpecificationList_InsertRange(swigCPtr, index, MessageSpecificationList.getCPtr(values));
    if (GmsecPINVOKE.SWIGPendingException.Pending) throw GmsecPINVOKE.SWIGPendingException.Retrieve();
  }

  public void RemoveAt(int index) {
    GmsecPINVOKE.MessageSpecificationList_RemoveAt(swigCPtr, index);
    if (GmsecPINVOKE.SWIGPendingException.Pending) throw GmsecPINVOKE.SWIGPendingException.Retrieve();
  }

  public void RemoveRange(int index, int count) {
    GmsecPINVOKE.MessageSpecificationList_RemoveRange(swigCPtr, index, count);
    if (GmsecPINVOKE.SWIGPendingException.Pending) throw GmsecPINVOKE.SWIGPendingException.Retrieve();
  }

  public static MessageSpecificationList Repeat(MessageSpecification value, int count) {
    global::System.IntPtr cPtr = GmsecPINVOKE.MessageSpecificationList_Repeat(MessageSpecification.getCPtr(value), count);
    MessageSpecificationList ret = (cPtr == global::System.IntPtr.Zero) ? null : new MessageSpecificationList(cPtr, true);
    if (GmsecPINVOKE.SWIGPendingException.Pending) throw GmsecPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public void Reverse() {
    GmsecPINVOKE.MessageSpecificationList_Reverse__SWIG_0(swigCPtr);
    if (GmsecPINVOKE.SWIGPendingException.Pending) throw GmsecPINVOKE.SWIGPendingException.Retrieve();
  }

  public void Reverse(int index, int count) {
    GmsecPINVOKE.MessageSpecificationList_Reverse__SWIG_1(swigCPtr, index, count);
    if (GmsecPINVOKE.SWIGPendingException.Pending) throw GmsecPINVOKE.SWIGPendingException.Retrieve();
  }

  public void SetRange(int index, MessageSpecificationList values) {
    GmsecPINVOKE.MessageSpecificationList_SetRange(swigCPtr, index, MessageSpecificationList.getCPtr(values));
    if (GmsecPINVOKE.SWIGPendingException.Pending) throw GmsecPINVOKE.SWIGPendingException.Retrieve();
  }

  public bool Contains(MessageSpecification value) {
    bool ret = GmsecPINVOKE.MessageSpecificationList_Contains(swigCPtr, MessageSpecification.getCPtr(value));
    if (GmsecPINVOKE.SWIGPendingException.Pending) throw GmsecPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public int IndexOf(MessageSpecification value) {
    int ret = GmsecPINVOKE.MessageSpecificationList_IndexOf(swigCPtr, MessageSpecification.getCPtr(value));
    if (GmsecPINVOKE.SWIGPendingException.Pending) throw GmsecPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public int LastIndexOf(MessageSpecification value) {
    int ret = GmsecPINVOKE.MessageSpecificationList_LastIndexOf(swigCPtr, MessageSpecification.getCPtr(value));
    if (GmsecPINVOKE.SWIGPendingException.Pending) throw GmsecPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public void Remove(MessageSpecification value) {
    GmsecPINVOKE.MessageSpecificationList_Remove(swigCPtr, MessageSpecification.getCPtr(value));
    if (GmsecPINVOKE.SWIGPendingException.Pending) throw GmsecPINVOKE.SWIGPendingException.Retrieve();
  }

}

}
