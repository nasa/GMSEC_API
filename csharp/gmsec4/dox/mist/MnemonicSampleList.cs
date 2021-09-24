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

/// <summary>Class that is used to manage a collection of MnemonicSample objects.</summary>

public class MnemonicSampleList : System.IDisposable, System.Collections.IEnumerable {

  public MnemonicSampleList(System.Collections.ICollection c) : this() {
    if (c == null)
      throw new System.ArgumentNullException("c");
    foreach (MnemonicSample element in c) {
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

  public MnemonicSample this[int index]  {
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
    return new MnemonicSampleListEnumerator(this);
  }

  public MnemonicSampleListEnumerator GetEnumerator() {
    return new MnemonicSampleListEnumerator(this);
  }

  // Type-safe enumerator
  /// Note that the IEnumerator documentation requires an InvalidOperationException to be thrown
  /// whenever the collection is modified. This has been done for changes in the size of the
  /// collection but not when one of the elements of the collection is modified as it is a bit
  /// tricky to detect unmanaged code that modifies the collection under our feet.
  public sealed class MnemonicSampleListEnumerator : System.Collections.IEnumerator {
    private MnemonicSampleList collectionRef;
    private int currentIndex;
    private object currentObject;
    private int currentSize;

    public MnemonicSampleListEnumerator(MnemonicSampleList collection) {
      collectionRef = collection;
      currentIndex = -1;
      currentObject = null;
      currentSize = collectionRef.Count;
    }

    // Type-safe iterator Current
    public MnemonicSample Current {
      get {
        if (currentIndex == -1)
          throw new System.InvalidOperationException("Enumeration not started.");
        if (currentIndex > currentSize - 1)
          throw new System.InvalidOperationException("Enumeration finished.");
        if (currentObject == null)
          throw new System.InvalidOperationException("Collection modified.");
        return (MnemonicSample)currentObject;
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
    GmsecPINVOKE.MnemonicSampleList_Clear(swigCPtr);
    if (GmsecPINVOKE.SWIGPendingException.Pending) throw GmsecPINVOKE.SWIGPendingException.Retrieve();
  }

  public void Add(MnemonicSample value) {
    GmsecPINVOKE.MnemonicSampleList_Add(swigCPtr, MnemonicSample.getCPtr(value));
    if (GmsecPINVOKE.SWIGPendingException.Pending) throw GmsecPINVOKE.SWIGPendingException.Retrieve();
  }

  private int size() {
    int ret = GmsecPINVOKE.MnemonicSampleList_size(swigCPtr);
    if (GmsecPINVOKE.SWIGPendingException.Pending) throw GmsecPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public MnemonicSampleList() : this(GmsecPINVOKE.new_MnemonicSampleList(), true) {
    if (GmsecPINVOKE.SWIGPendingException.Pending) throw GmsecPINVOKE.SWIGPendingException.Retrieve();
  }

  private MnemonicSample getitemcopy(int index) {
    global::System.IntPtr cPtr = GmsecPINVOKE.MnemonicSampleList_getitemcopy(swigCPtr, index);
    MnemonicSample ret = (cPtr == global::System.IntPtr.Zero) ? null : new MnemonicSample(cPtr, false);
    if (GmsecPINVOKE.SWIGPendingException.Pending) throw GmsecPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  private MnemonicSample getitem(int index) {
    global::System.IntPtr cPtr = GmsecPINVOKE.MnemonicSampleList_getitem(swigCPtr, index);
    MnemonicSample ret = (cPtr == global::System.IntPtr.Zero) ? null : new MnemonicSample(cPtr, false);
    if (GmsecPINVOKE.SWIGPendingException.Pending) throw GmsecPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public void AddRange(MnemonicSampleList values) {
    GmsecPINVOKE.MnemonicSampleList_AddRange(swigCPtr, MnemonicSampleList.getCPtr(values));
    if (GmsecPINVOKE.SWIGPendingException.Pending) throw GmsecPINVOKE.SWIGPendingException.Retrieve();
  }

  public MnemonicSampleList GetRange(int index, int count) {
    global::System.IntPtr cPtr = GmsecPINVOKE.MnemonicSampleList_GetRange(swigCPtr, index, count);
    MnemonicSampleList ret = (cPtr == global::System.IntPtr.Zero) ? null : new MnemonicSampleList(cPtr, true);
    if (GmsecPINVOKE.SWIGPendingException.Pending) throw GmsecPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public void Insert(int index, MnemonicSample value) {
    GmsecPINVOKE.MnemonicSampleList_Insert(swigCPtr, index, MnemonicSample.getCPtr(value));
    if (GmsecPINVOKE.SWIGPendingException.Pending) throw GmsecPINVOKE.SWIGPendingException.Retrieve();
  }

  public void InsertRange(int index, MnemonicSampleList values) {
    GmsecPINVOKE.MnemonicSampleList_InsertRange(swigCPtr, index, MnemonicSampleList.getCPtr(values));
    if (GmsecPINVOKE.SWIGPendingException.Pending) throw GmsecPINVOKE.SWIGPendingException.Retrieve();
  }

  public void RemoveAt(int index) {
    GmsecPINVOKE.MnemonicSampleList_RemoveAt(swigCPtr, index);
    if (GmsecPINVOKE.SWIGPendingException.Pending) throw GmsecPINVOKE.SWIGPendingException.Retrieve();
  }

  public void RemoveRange(int index, int count) {
    GmsecPINVOKE.MnemonicSampleList_RemoveRange(swigCPtr, index, count);
    if (GmsecPINVOKE.SWIGPendingException.Pending) throw GmsecPINVOKE.SWIGPendingException.Retrieve();
  }

  public static MnemonicSampleList Repeat(MnemonicSample value, int count) {
    global::System.IntPtr cPtr = GmsecPINVOKE.MnemonicSampleList_Repeat(MnemonicSample.getCPtr(value), count);
    MnemonicSampleList ret = (cPtr == global::System.IntPtr.Zero) ? null : new MnemonicSampleList(cPtr, true);
    if (GmsecPINVOKE.SWIGPendingException.Pending) throw GmsecPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public void Reverse() {
    GmsecPINVOKE.MnemonicSampleList_Reverse__SWIG_0(swigCPtr);
    if (GmsecPINVOKE.SWIGPendingException.Pending) throw GmsecPINVOKE.SWIGPendingException.Retrieve();
  }

  public void Reverse(int index, int count) {
    GmsecPINVOKE.MnemonicSampleList_Reverse__SWIG_1(swigCPtr, index, count);
    if (GmsecPINVOKE.SWIGPendingException.Pending) throw GmsecPINVOKE.SWIGPendingException.Retrieve();
  }

  public void SetRange(int index, MnemonicSampleList values) {
    GmsecPINVOKE.MnemonicSampleList_SetRange(swigCPtr, index, MnemonicSampleList.getCPtr(values));
    if (GmsecPINVOKE.SWIGPendingException.Pending) throw GmsecPINVOKE.SWIGPendingException.Retrieve();
  }

  public bool Contains(MnemonicSample value) {
    bool ret = GmsecPINVOKE.MnemonicSampleList_Contains(swigCPtr, MnemonicSample.getCPtr(value));
    if (GmsecPINVOKE.SWIGPendingException.Pending) throw GmsecPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public int IndexOf(MnemonicSample value) {
    int ret = GmsecPINVOKE.MnemonicSampleList_IndexOf(swigCPtr, MnemonicSample.getCPtr(value));
    if (GmsecPINVOKE.SWIGPendingException.Pending) throw GmsecPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public int LastIndexOf(MnemonicSample value) {
    int ret = GmsecPINVOKE.MnemonicSampleList_LastIndexOf(swigCPtr, MnemonicSample.getCPtr(value));
    if (GmsecPINVOKE.SWIGPendingException.Pending) throw GmsecPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public void Remove(MnemonicSample value) {
    GmsecPINVOKE.MnemonicSampleList_Remove(swigCPtr, MnemonicSample.getCPtr(value));
    if (GmsecPINVOKE.SWIGPendingException.Pending) throw GmsecPINVOKE.SWIGPendingException.Retrieve();
  }

}

}
