namespace Utils{
	template<typename T>
	class IterArray;
	template<typename T>
	class Array{
	private:
		T * Data;
		unsigned long AllocNum;
	public:
		Array();
		Array(Array<T> &&Cpy);
		Array(const T *StrIn, unsigned long Len);
		Array(const Array<T> &Cpy);
		Array(const T ChFill, const unsigned long Len);
		Array(const Array<T> &Cpy, unsigned long Len);
		~Array();
		void Swap(Array<T> &Other);
		void AddMissing(const Array<T> &Other);
		void AddMissing(const Array<T> &Other, unsigned long Until);
		void RemBeg(unsigned long NumRem);
		void AddBeg(unsigned long NumAdd);
		void AddBeg(unsigned long NumAdd, T Val);
		void SetLength(unsigned long Len);
		bool operator==(const Array<T> &Cmp) const;
		bool operator!=(const Array<T> &Cmp) const;
		Array<T> operator+(const Array<T> &Add) const;
		Array<T> operator+(const T &Add) const;
		T operator[](const unsigned long Pos) const;
		Array<T> &operator=(Array<T> &&Cpy);
		Array<T> &operator=(const Array<T> &Cpy);
		Array<T> &operator+=(const Array<T> &Add);
		Array<T> &operator+=(const T &Add);
		//less cpu intensive for higher performance. NOTE: do NOT deallocate or the Array object becomes invalid
		const T *GetData() const;
		bool Insert(unsigned long Pos, T Val);
		bool Remove(unsigned long Pos);
		T &operator[](const unsigned long Pos);
		bool Find(unsigned long &Pos, T Val, bool PosIsStart = false);
		bool RFind(unsigned long &Pos, T Val, bool PosIsStart = false);
		unsigned long Length() const;
		T &AtEnd();
		const T AtEnd() const;
		Array<T> &operator*=(unsigned long Num);
		Array<T> operator*(unsigned long Num) const;
		IterArray<T> begin();
		IterArray<T> end();
		//friend class ReprArray;
	};
	template<typename T>
	class IterArray{
	private:
		Array<T> *Str;
		unsigned long Pos;
	public:
		IterArray(Array<T> *Iter, unsigned long CharPos);
		IterArray<T> &operator++();
		bool operator==(IterArray<T> &Cmp);
		bool operator!=(IterArray<T> &Cmp);
		T &operator*();
	};
	template<typename T>
	void Swap(T &A, T &B){
		T C = A;
		A = B;
		B = C;
	}
	template<typename T>
	Array<T>::Array(){
		Data = 0;
		AllocNum = 0;
	}

	template<typename T>
	Array<T>::Array(Array<T> &&Cpy){
		Data = Cpy.Data;
		Cpy.Data = 0;
		AllocNum = Cpy.AllocNum;
		Cpy.AllocNum = 0;
	}

	template<typename T>
	Array<T>::Array(const T *Cpy, unsigned long Len){
		AllocNum = Len;
		Data = new T[AllocNum];
		unsigned long c = 0;
		while (c < AllocNum){
			Data[c] = Cpy[c];
			++c;
		}
	}

	template<typename T>
	Array<T>::Array(const Array<T> &Cpy){
		AllocNum = Cpy.AllocNum;
		Data = new T[AllocNum];
		unsigned long c = 0;
		while (c < AllocNum){
			Data[c] = Cpy.Data[c];
			++c;
		}
	}

	template<typename T>
	Array<T>::Array(const T ChFill, const unsigned long Len){
		AllocNum = Len;
		Data = new T[AllocNum];
		unsigned long c = 0;
		while (c < AllocNum){
			Data[c] = ChFill;
			++c;
		}
	}

	template<typename T>
	Array<T>::Array(const Array<T> &Cpy, unsigned long Len){
		AllocNum = (Len < Cpy.AllocNum) ? Len : Cpy.AllocNum;
		Data = new T[AllocNum];
		unsigned long c = 0;
		while (c < AllocNum){
			Data[c] = Cpy.Data[c];
			++c;
		}
	}

	template<typename T>
	Array<T>::~Array(){
		if (AllocNum > 0) delete[] Data;
	}

	template<typename T>
	void Array<T>::Swap(Array<T> &Other){
		T *TmpData = Data;
		unsigned long TmpLen = AllocNum;
		AllocNum = Other.AllocNum;
		Data = Other.Data;
		Other.AllocNum = TmpLen;
		Other.Data = TmpData;
	}
	template<typename T>
	void Array<T>::SetLength(unsigned long Len){
		T *NewData = new T[Len];
		for (unsigned long c = 0; (c < Len) && (c < AllocNum); ++c){
			NewData[c] = Data[c];
		}
		delete[] Data;
		Data = NewData;
		AllocNum = Len;
	}

	template<typename T>
	void Array<T>::AddMissing(const Array<T> &Other){
		if (Other.AllocNum <= AllocNum) return;
		T * NewData = new T[Other.AllocNum];
		unsigned long c = 0;
		while (c < AllocNum){
			NewData[c] = Data[c];
			++c;
		}
		while (c < Other.AllocNum){
			NewData[c] = Other.Data[c];
			++c;
		}
		delete[] Data;
		Data = NewData;
		AllocNum = Other.AllocNum;
	}
	template<typename T>
	void Array<T>::AddMissing(const Array<T> &Other, unsigned long Until){
		if (Until <= AllocNum) return;
		T * NewData = new T[Other.AllocNum];
		unsigned long c = 0;
		while (c < AllocNum){
			NewData[c] = Data[c];
			++c;
		}
		while (c < Until){
			NewData[c] = Other.Data[c];
			++c;
		}
		delete[] Data;
		Data = NewData;
		AllocNum = Until;
	}
	template<typename T>
	void Array<T>::RemBeg(unsigned long NumRem){
		unsigned long c = 0, Until = AllocNum - NumRem;
		T * NewData = new T[Until];
		Data += Until;
		while (c < Until){
			NewData[c] = Data[c];
		}
		Data -= Until;
		delete[] Data;
		Data = NewData;
		AllocNum = Until;
	}
	template<typename T>
	void Array<T>::AddBeg(unsigned long NumAdd){
		if (NumAdd == 0) return;
		T * NewData = new T[AllocNum + NumAdd];
		unsigned long c = 0;
		NewData += NumAdd;
		while (c < AllocNum){
			NewData[c] = Data[c];
			++c;
		}
		delete[] Data;
		NewData -= NumAdd;
		AllocNum += NumAdd;
		Data = NewData;
	}
	template<typename T>
	void Array<T>::AddBeg(unsigned long NumAdd, T Val){
		if (NumAdd == 0) return;
		T * NewData = new T[AllocNum + NumAdd];
		unsigned long c = 0;
		NewData += NumAdd;
		while (c < AllocNum){
			NewData[c] = Data[c];
			++c;
		}
		delete[] Data;
		NewData -= NumAdd;
		c = 0;
		while (c < NumAdd){
			NewData[c] = Val;
			++c;
		}
		AllocNum += NumAdd;
		Data = NewData;
	}

	template<typename T>
	bool Array<T>::operator==(const Array<T> &Cmp) const{
		if (AllocNum != Cmp.AllocNum) return false;
		unsigned long c = 0;
		while (c < AllocNum){
			if (Data[c] != Cmp.Data[c]) return false;
			++c;
		}
		return true;
	}

	template<typename T>
	bool Array<T>::operator!=(const Array<T> &Cmp) const{
		if (AllocNum != Cmp.AllocNum) return true;
		unsigned long c = 0;
		while (c < AllocNum){
			if (Data[c] != Cmp.Data[c]) return true;
			++c;
		}
		return false;
	}

	template<typename T>
	Array<T> Array<T>::operator+(const Array<T> &Add) const{
		Array<T> Rtn;
		Rtn.AllocNum = AllocNum + Add.AllocNum;
		Rtn.Data = new T[Rtn.AllocNum];
		unsigned long c = 0;
		while (c < AllocNum){
			Rtn.Data[c] = Data[c];
			++c;
		}
		while (c < Rtn.AllocNum){
			Rtn.Data[c] = Add.Data[c - AllocNum];
			++c;
		}
		return Rtn;
	}

	template<typename T>
	Array<T> Array<T>::operator+(const T &Add) const{
		Array<T> Rtn;
		Rtn.AllocNum = AllocNum + 1;
		Rtn.Data = new T[Rtn.AllocNum];
		unsigned long c = 0;
		while (c < AllocNum){
			Rtn.Data[c] = Data[c];
			++c;
		}
		Rtn.Data[c] = Add;
		return Rtn;
	}

	template<typename T>
	T Array<T>::operator[](const unsigned long Pos) const{
		if (Pos >= AllocNum) return T();
		return Data[Pos];
	}

	template<typename T>
	Array<T> &Array<T>::operator=(Array<T> &&Cpy){
		if (AllocNum > 0) delete[] Data;
		Data = Cpy.Data;
		Cpy.Data = 0;
		AllocNum = Cpy.AllocNum;
		Cpy.AllocNum = 0;
		return (*this);
	}

	template<typename T>
	Array<T> &Array<T>::operator=(const Array<T> &Cpy){
		if (AllocNum > 0) delete[] Data;
		AllocNum = Cpy.AllocNum;
		Data = new T[AllocNum];
		unsigned long c = 0;
		while (c < AllocNum){
			Data[c] = Cpy.Data[c];
			++c;
		}
		return (*this);
	}

	template<typename T>
	Array<T> &Array<T>::operator+=(const Array<T> &Add){
		T *NewData = new T[AllocNum + Add.AllocNum];
		unsigned long c = 0;
		while (c < AllocNum){
			NewData[c] = Data[c];
			++c;
		}
		if (AllocNum > 0) delete[] Data;
		const unsigned long PrevAllocNum = AllocNum;
		AllocNum += Add.AllocNum;
		while (c < AllocNum){
			NewData[c] = Add.Data[c - PrevAllocNum];
			++c;
		}
		Data = NewData;
		return (*this);
	}

	template<typename T>
	Array<T> &Array<T>::operator+=(const T &Add){
		T *NewData = new T[AllocNum + 1];
		unsigned long c = 0;
		while (c < AllocNum){
			NewData[c] = Data[c];
			++c;
		}
		if (AllocNum > 0) delete[] Data;
		NewData[c] = Add;
		Data = NewData;
		++AllocNum;
		return (*this);
	}

	//less cpu intensive for higher performance. NOTE: do NOT deallocate or the string object becomes invalid
	template<typename T>
	const T *Array<T>::GetData() const{
		return Data;
	}

	template<typename T>
	bool Array<T>::Insert(unsigned long Pos, T Val){
		if (Pos > AllocNum) return false;
		T *NewData = new T[AllocNum + 1];
		++AllocNum;
		unsigned long c = 0;
		while (c < Pos){
			NewData[c] = Data[c];
			++c;
		}
		NewData[c] = Val;
		++c;
		while (c < AllocNum){
			NewData[c] = Data[c - 1];
			++c;
		}
		delete[] Data;
		Data = NewData;
		return true;
	}

	template<typename T>
	bool Array<T>::Remove(unsigned long Pos){
		if (Pos >= AllocNum) return false;
		T *NewData = new T[AllocNum - 1];
		--AllocNum;
		unsigned long c = 0;
		while (c < Pos){
			NewData[c] = Data[c];
			++c;
		}
		while (c < AllocNum){
			NewData[c] = Data[c + 1];
			++c;
		}
		delete[] Data;
		Data = NewData;
		return true;
	}

	template<typename T>
	T &Array<T>::operator[](const unsigned long Pos){
		return Data[Pos];
	}

	template<typename T>
	bool Array<T>::Find(unsigned long &Pos, T Val, bool PosIsStart){
		for (unsigned long c = PosIsStart ? Pos : 0; c < AllocNum; ++c){
			if (Data[c] == Val)
			{
				Pos = c;
				return true;
			}
		}
		return false;
	}
	template<typename T>
	bool Array<T>::RFind(unsigned long &Pos, T Val, bool PosIsStart){
		--Data;//to shift Data so that (Old)Data[0] is the same as (New)Data[1]
		unsigned long Until = PosIsStart ? Pos : 0;
		for (unsigned long c = AllocNum; c > Until; --c){
			if (Data[c] == Val)
			{
				Pos = c;
				++Data;
				return true;
			}
		}
		++Data;
		return false;
	}
	template<typename T>
	unsigned long Array<T>::Length() const{
		return AllocNum;
	}

	template<typename T>
	T &Array<T>::AtEnd(){
		return Data[AllocNum - 1];
	}

	template<typename T>
	const T Array<T>::AtEnd() const{
		return Data[AllocNum - 1];
	}

	template<typename T>
	Array<T> &Array<T>::operator*=(unsigned long Num){
		T *NewData = new T[Num * AllocNum];
		unsigned long c = 0;
		while (c < Num){
			unsigned long c1 = 0;
			while (c1 < AllocNum){
				NewData[(c * AllocNum) + c1] = Data[c1];
				++c1;
			}
			++c;
		}
		delete[] Data;
		Data = NewData;
		AllocNum *= Num;
		return (*this);
	}

	template<typename T>
	Array<T> Array<T>::operator*(unsigned long Num) const{
		Array<T> Rtn;
		Rtn.Data = new T[Num * AllocNum];
		unsigned long c = 0;
		while (c < Num){
			unsigned long c1 = 0;
			while (c1 < AllocNum){
				Rtn.Data[(c * AllocNum) + c1] = Data[c1];
				++c1;
			}
			++c;
		}
		Rtn.AllocNum = Num * AllocNum;
		return Rtn;
	}
	template<typename T>
	IterArray<T> Array<T>::begin(){
		return IterArray<T>(this, 0);
	}
	template<typename T>
	IterArray<T> Array<T>::end(){
		return IterArray<T>(this, AllocNum);
	}

	template<typename T>
	IterArray<T>::IterArray(Array<T> *Iter, unsigned long Index){
		Str = Iter;
		Pos = Index;
	}
	template<typename T>
	IterArray<T> &IterArray<T>::operator++(){
		++Pos;
		return (*this);
	}
	template<typename T>
	bool IterArray<T>::operator==(IterArray<T> &Cmp){
		return (Pos == Cmp.Pos) && (Str == Cmp.Str);
	}
	template<typename T>
	bool IterArray<T>::operator!=(IterArray<T> &Cmp){
		return (Pos != Cmp.Pos) || (Str != Cmp.Str);
	}
	template<typename T>
	T &IterArray<T>::operator*(){
		return (*Str)[Pos];
	}
	template<typename T1, typename T2>
	class HashMap {
	public:
		typedef unsigned long(*HashFunc)(T1 &, unsigned long);
		struct DataPair {
			T1 Key;
			T2 Val;
			DataPair *Next;
			bool IsValid;
			DataPair();
			DataPair(T1 KeyVal, T2 Value, DataPair *next = 0);
			DataPair(const T1 &KeyVal, const T2 &Value, DataPair *next = 0);
		};
	private:
		HashFunc Hasher;
		DataPair *Datas;
		unsigned long AllocNum;
		//returns false if the key holder is in the root list: pointer points to it, returns true if not: pointer points to previous key holder in linked list
		bool GetWithKey(const T1 &KeyVal, DataPair *&PtrPrev);
	public:
		HashMap(HashFunc hasher = 0);
		T2 &AtKey(const T1 &KeyVal);
		const T2 Get(const T1 &KeyVal) const;
		void Put(const T1 &KeyVal, const T2 &Value);
		bool Rem(const T1 &KeyVal);
		void PreAlloc(unsigned long Num);
		unsigned long NumHashes();
		const T1 *FindKey(const T2 &Value) const;
		Array<DataPair *> GetKeyPairs() const;
	};
	template<typename T1, typename T2>
	HashMap<T1, T2>::DataPair::DataPair() {
		Next = 0;
		IsValid = false;
	}
	template<typename T1, typename T2>
	HashMap<T1, T2>::DataPair::DataPair(const T1 &KeyVal, const T2 &Value, DataPair *next = 0) {
		Key = KeyVal;
		Val = Value;
		Next = next;
		IsValid = true;
	}
	template<typename T1, typename T2>
	HashMap<T1, T2>::DataPair::DataPair(T1 KeyVal, T2 Value, DataPair *next = 0) {
		Key = KeyVal;
		Val = Value;
		Next = next;
		IsValid = true;
	}
	template<typename T1, typename T2>
	HashMap<T1, T2>::HashMap(HashFunc hasher = 0) {
		Hasher = hasher;
		AllocNum = 8;
		Datas = new DataPair[AllocNum];
	}
	template<typename T1, typename T2>
	T2 &HashMap<T1, T2>::AtKey(const T1 &KeyVal) {
		DataPair *GetPair = 0;
		bool Tmp = GetWithKey(KeyVal, GetPair);
		if (Tmp) GetPair = GetPair->Next;
		return GetPair->Val;
	}
	template<typename T1, typename T2>
	const T2 HashMap<T1, T2>::Get(const T1 &KeyVal) const {
		DataPair *GetPair = 0;
		bool Tmp = GetWithKey(KeyVal, GetPair);
		if (Tmp) GetPair = GetPair->Next;
		return GetPair->Val;
	}
	template<typename T1, typename T2>
	void HashMap<T1, T2>::Put(const T1 &KeyVal, const T2 &Value) {
		DataPair *GetPair = 0;
		bool Tmp = GetWithKey(KeyVal, GetPair);
		if (GetPair != 0)
		{
			if (Tmp) GetPair = GetPair->Next;
			GetPair->Val = Value;
		}
		else
		{
			unsigned long Pos = Hasher(KeyVal, AllocNum);
			GetPair = Datas[Pos];
			if (Datas[Pos].IsValid) GetPair = Datas[Pos]->Next;
			else
			{
				Datas[Pos].Key = KeyVal;
				Datas[Pos].Val = Value;
				Datas[Pos].IsValid = true;
				return;
			}
			while (GetPair->Next != 0)
				GetPair = GetPair->Next;
			GetPair->Next = new DataPair(KeyVal, Value);
		}
	}
	template<typename T1, typename T2>
	bool HashMap<T1, T2>::Rem(const T1 &KeyVal) {
		DataPair *GetPair = 0;
		bool Tmp = GetWithKey(KeyVal, GetPair);
		if (Tmp)
		{
			DataPair * OrigNext = GetPair->Next;
			GetPair->Next = OrigNext->Next;
			delete OrigNext;
		}
		else
		{
			if (GetPair->Next == 0)
			{
				GetPair->Key = T1();
				GetPair->Val = T2();
				GetPair->IsValid = false;
			}
			else
			{
				DataPair OrigNext = GetPair->Next;
				GetPair->Next = OrigNext->Next;
				//we are basically moving from OrigNext to GetPair so to be efficient we will use move assignment
				GetPair->Key.operator=((T1 &&)OrigNext->Key);
				GetPair->Val.operator=((T2 &&)OrigNext->Val);
				//since we MOVED from OrigNext, deleting OrigNext shouldn't invalidate GetPair's copies of Key and Val
				delete OrigNext;
			}
		}
	}
	template<typename T1, typename T2>
	void HashMap<T1, T2>::PreAlloc(unsigned long Num) {
		//basically this is rehashing the entire map with a different number of hashes
		DataPair *NewDatas = new DataPair[Num];
		Array<DataPair *> OrigPairs = (Array<DataPair *>)GetKeyPairs();
		for (DataPair *&Pair : OrigPairs) {
			unsigned long Pos = Hasher(Pair->Key, Num);
			DataPair *NewPair = &NewDatas[Pos];
			if (NewDatas[Pos].IsValid)
			{
				while (NewPair->Next != 0) NewPair = NewPair->Next;
				NewPair->Next = new DataPair();
				NewPair = NewPair->Next;
			}
			NewPair->Key.operator=((T1 &&)Pair->Key);
			NewPair->Val.operator=((T2 &&)Pair->Val);
			NewPair->IsValid = true;
		}
		//deallocate the pairs that not directly in the array of pairs
		unsigned long c = OrigPairs.Length();
		while (c > 0)
		{
			--c;
			if ((unsigned long)(OrigPairs[c] - Datas) >= AllocNum)
			{
				delete OrigPairs[c];
			}
		}
		//deallocate the array of pairs
		delete[] Datas;
		//set the array of pairs to the new array
		AllocNum = Num;
		Datas = NewDatas;
	}
	template<typename T1, typename T2>
	unsigned long HashMap<T1, T2>::NumHashes() {
		unsigned long Rtn = 0;
		for (unsigned long c = 0; c < AllocNum; ++c) if (Datas[c].IsValid) ++Rtn;
		return Rtn;
	}
	template<typename T1, typename T2>
	const T1 *HashMap<T1, T2>::FindKey(const T2 &Value) const {
		for (const DataPair *Pair : GetKeyPairs()) {
			if (Pair->Val == Value) return &Pair->Key;
		}
		return 0;
	}
	template<typename T1, typename T2>
	Array<typename HashMap<T1, T2>::DataPair *> HashMap<T1, T2>::GetKeyPairs() const {
		unsigned long NumDats = 0;
		Array<DataPair *> DataPairs;
		for (unsigned long c = 0; c < AllocNum; ++c) {
			if (!Datas[c].IsValid) continue;
			++NumDats;
			DataPair *Cur = &Datas[c];
			while (Cur->Next != 0) {
				Cur = Cur->Next;
				++NumDats;
			}
		}
		DataPairs.SetLength(NumDats);
		IterArray<DataPair *> IterPairs = DataPairs.begin();
		for (unsigned long c = 0; c < AllocNum; ++c) {
			if (!Datas[c].IsValid) continue;
			*IterPairs = Datas[c];
			++IterPairs;
			DataPair *Cur = &Datas[c];
			while (Cur->Next != 0) {
				Cur = Cur->Next;
				*IterPairs = Cur;
				++IterPairs;
			}
		}
		return DataPairs;
	}
	template<typename T1, typename T2>
	bool HashMap<T1, T2>::GetWithKey(const T1 &KeyVal, DataPair *&PtrPrev) {
		unsigned long Pos = Hasher(KeyVal, AllocNum);
		PtrPrev = 0;
		if (!Datas[Pos].IsValid) return false;
		PtrPrev = Datas[Pos];
		if (Datas[Pos].Key == KeyVal) return false;
		while (PtrPrev->Next != 0) {
			DataPair *Cur = PtrPrev->Next;
			if (Cur->Key == KeyVal) return true;
			PtrPrev = Cur;
		}
		PtrPrev = 0;
		return false;
	}
}