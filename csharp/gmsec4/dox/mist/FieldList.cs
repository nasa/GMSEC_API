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

/// <summary>Class that is used to manage a collection of Field objects.</summary>

public class FieldList : System.IDisposable, System.Collections.IEnumerable {

  public FieldList(System.Collections.ICollection c) : this() {
    if (c == null)
      throw new System.ArgumentNullException("c");
    foreach (Field element in c) {
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

  public Field this[int index]  {
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
    return new FieldListEnumerator(this);
  }

  public FieldListEnumerator GetEnumerator() {
    return new FieldListEnumerator(this);
  }

  // Type-safe enumerator
  /// Note that the IEnumerator documentation requires an InvalidOperationException to be thrown
  /// whenever the collection is modified. This has been done for changes in the size of the
  /// collection but not when one of the elements of the collection is modified as it is a bit
  /// tricky to detect unmanaged code that modifies the collection under our feet.
  public sealed class FieldListEnumerator : System.Collections.IEnumerator {
    private FieldList collectionRef;
    private int currentIndex;
    private object currentObject;
    private int currentSize;

    public FieldListEnumerator(FieldList collection) {
      collectionRef = collection;
      currentIndex = -1;
      currentObject = null;
      currentSize = collectionRef.Count;
    }

    // Type-safe iterator Current
    public Field Current {
      get {
        if (currentIndex == -1)
          throw new System.InvalidOperationException("Enumeration not started.");
        if (currentIndex > currentSize - 1)
          throw new System.InvalidOperationException("Enumeration finished.");
        if (currentObject == null)
          throw new System.InvalidOperationException("Collection modified.");
        return (Field)currentObject;
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
    GmsecPINVOKE.FieldList_Clear(swigCPtr);
    if (GmsecPINVOKE.SWIGPendingException.Pending) throw GmsecPINVOKE.SWIGPendingException.Retrieve();
  }

  public void Add(Field value) {
    GmsecPINVOKE.FieldList_Add(swigCPtr, Field.getCPtr(value));
    if (GmsecPINVOKE.SWIGPendingException.Pending) throw GmsecPINVOKE.SWIGPendingException.Retrieve();
  }

  private int size() {
    int ret = GmsecPINVOKE.FieldList_size(swigCPtr);
    if (GmsecPINVOKE.SWIGPendingException.Pending) throw GmsecPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public FieldList() : this(GmsecPINVOKE.new_FieldList(), true) {
    if (GmsecPINVOKE.SWIGPendingException.Pending) throw GmsecPINVOKE.SWIGPendingException.Retrieve();
  }

  private Field getitemcopy(int index) {
    global::System.IntPtr cPtr = GmsecPINVOKE.FieldList_getitemcopy(swigCPtr, index);
    Field ret = (cPtr == global::System.IntPtr.Zero) ? null : FieldFactory.BuildField(cPtr, false);
    if (GmsecPINVOKE.SWIGPendingException.Pending) throw GmsecPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  private Field getitem(int index) {
    global::System.IntPtr cPtr = GmsecPINVOKE.FieldList_getitem(swigCPtr, index);
    Field ret = (cPtr == global::System.IntPtr.Zero) ? null : FieldFactory.BuildField(cPtr, false);
    if (GmsecPINVOKE.SWIGPendingException.Pending) throw GmsecPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public void AddRange(FieldList values) {
    GmsecPINVOKE.FieldList_AddRange(swigCPtr, FieldList.getCPtr(values));
    if (GmsecPINVOKE.SWIGPendingException.Pending) throw GmsecPINVOKE.SWIGPendingException.Retrieve();
  }

  public FieldList GetRange(int index, int count) {
    global::System.IntPtr cPtr = GmsecPINVOKE.FieldList_GetRange(swigCPtr, index, count);
    FieldList ret = (cPtr == global::System.IntPtr.Zero) ? null : new FieldList(cPtr, true);
    if (GmsecPINVOKE.SWIGPendingException.Pending) throw GmsecPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public void Insert(int index, Field value) {
    GmsecPINVOKE.FieldList_Insert(swigCPtr, index, Field.getCPtr(value));
    if (GmsecPINVOKE.SWIGPendingException.Pending) throw GmsecPINVOKE.SWIGPendingException.Retrieve();
  }

  public void InsertRange(int index, FieldList values) {
    GmsecPINVOKE.FieldList_InsertRange(swigCPtr, index, FieldList.getCPtr(values));
    if (GmsecPINVOKE.SWIGPendingException.Pending) throw GmsecPINVOKE.SWIGPendingException.Retrieve();
  }

  public void RemoveAt(int index) {
    GmsecPINVOKE.FieldList_RemoveAt(swigCPtr, index);
    if (GmsecPINVOKE.SWIGPendingException.Pending) throw GmsecPINVOKE.SWIGPendingException.Retrieve();
  }

  public void RemoveRange(int index, int count) {
    GmsecPINVOKE.FieldList_RemoveRange(swigCPtr, index, count);
    if (GmsecPINVOKE.SWIGPendingException.Pending) throw GmsecPINVOKE.SWIGPendingException.Retrieve();
  }

  public static FieldList Repeat(Field value, int count) {
    global::System.IntPtr cPtr = GmsecPINVOKE.FieldList_Repeat(Field.getCPtr(value), count);
    FieldList ret = (cPtr == global::System.IntPtr.Zero) ? null : new FieldList(cPtr, true);
    if (GmsecPINVOKE.SWIGPendingException.Pending) throw GmsecPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public void Reverse() {
    GmsecPINVOKE.FieldList_Reverse__SWIG_0(swigCPtr);
    if (GmsecPINVOKE.SWIGPendingException.Pending) throw GmsecPINVOKE.SWIGPendingException.Retrieve();
  }

  public void Reverse(int index, int count) {
    GmsecPINVOKE.FieldList_Reverse__SWIG_1(swigCPtr, index, count);
    if (GmsecPINVOKE.SWIGPendingException.Pending) throw GmsecPINVOKE.SWIGPendingException.Retrieve();
  }

  public void SetRange(int index, FieldList values) {
    GmsecPINVOKE.FieldList_SetRange(swigCPtr, index, FieldList.getCPtr(values));
    if (GmsecPINVOKE.SWIGPendingException.Pending) throw GmsecPINVOKE.SWIGPendingException.Retrieve();
  }

  public bool Contains(Field value) {
    bool ret = GmsecPINVOKE.FieldList_Contains(swigCPtr, Field.getCPtr(value));
    if (GmsecPINVOKE.SWIGPendingException.Pending) throw GmsecPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public int IndexOf(Field value) {
    int ret = GmsecPINVOKE.FieldList_IndexOf(swigCPtr, Field.getCPtr(value));
    if (GmsecPINVOKE.SWIGPendingException.Pending) throw GmsecPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public int LastIndexOf(Field value) {
    int ret = GmsecPINVOKE.FieldList_LastIndexOf(swigCPtr, Field.getCPtr(value));
    if (GmsecPINVOKE.SWIGPendingException.Pending) throw GmsecPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public void Remove(Field value) {
    GmsecPINVOKE.FieldList_Remove(swigCPtr, Field.getCPtr(value));
    if (GmsecPINVOKE.SWIGPendingException.Pending) throw GmsecPINVOKE.SWIGPendingException.Retrieve();
  }

}

}
