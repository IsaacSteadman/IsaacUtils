#define MAX_INT64 0xFFFFFFFFFFFFFFFF
#define MAX_INT32 0xFFFFFFFF
#if defined(_WIN64)
typedef unsigned long long SizeL;
typedef signed long long SnzL; //signed version of SizeL
#define MAX_INT 0xFFFFFFFFFFFFFFFF
#define MAX_HEX_DIGIT 16
#else
typedef unsigned long SizeL;
typedef signed long SnzL;
#define MAX_INT 0xFFFFFFFF
#define MAX_HEX_DIGIT 8
#endif
namespace Utils{
	template<typename T>
	class IterArray;
	template<typename T>
	class Array{
	private:
		SizeL AllocNum;
		T * Data;
	public:
		Array();
		Array(Array<T> &&Cpy);
		Array(const T *StrIn, SizeL Len);
		Array(const Array<T> &Cpy);
		Array(const T ChFill, const SizeL Len);
		Array(const Array<T> &Cpy, SizeL Len);
		~Array();
		void Take(T *StrIn, SizeL Len);
		void Give(T *&StrOut, SizeL &Len);
		void Swap(Array<T> &Other);
		void AddMissing(const Array<T> &Other);
		void AddMissing(const Array<T> &Other, SizeL Until);
		void RemBeg(SizeL NumRem);
		void AddBeg(SizeL NumAdd);
		void AddBeg(SizeL NumAdd, T Val);
		void WriteFromAt(const Array<T> &From, SizeL Beg = 0, SizeL End = MAX_INT);
		Array<T> SubArr(SizeL Start, SizeL Stop = MAX_INT, SnzL Step = 0) const;
		void SetLength(SizeL Len);
		bool operator==(const Array<T> &Cmp) const;
		bool operator!=(const Array<T> &Cmp) const;
		Array<T> operator+(const Array<T> &Add) const;
		Array<T> operator+(const T &Add) const;
		T operator[](const SizeL Pos) const;
		Array<T> &operator=(Array<T> &&Cpy);
		Array<T> &operator=(const Array<T> &Cpy);
		Array<T> &operator+=(const Array<T> &Add);
		Array<T> &operator+=(const T &Add);
		//less cpu intensive for higher performance. NOTE: do NOT deallocate or the Array object becomes invalid
		const T *GetData() const;
		bool Insert(SizeL Pos, T Val);
		bool Remove(SizeL Pos);
		T &operator[](const SizeL Pos);
		bool Contains(const T &Val) const;
		bool Find(SizeL &Pos, T Val, bool PosIsStart = false);
		bool RFind(SizeL &Pos, T Val, bool PosIsStart = false);
		SizeL Length() const;
		T &AtEnd();
		const T AtEnd() const;
		Array<T> &operator*=(SizeL Num);
		Array<T> operator*(SizeL Num) const;
		IterArray<T> begin();
		IterArray<T> end();
		//friend class ReprArray;
	};
	template<typename T>
	class IterArray{
	private:
		Array<T> *Str;
		SizeL Pos;
	public:
		IterArray(Array<T> *Iter, SizeL CharPos);
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
	Array<T>::Array(const T *Cpy, SizeL Len){
		AllocNum = Len;
		Data = new T[AllocNum];
		SizeL c = 0;
		while (c < AllocNum){
			Data[c] = Cpy[c];
			++c;
		}
	}

	template<typename T>
	Array<T>::Array(const Array<T> &Cpy){
		AllocNum = Cpy.AllocNum;
		Data = new T[AllocNum];
		SizeL c = 0;
		while (c < AllocNum){
			Data[c] = Cpy.Data[c];
			++c;
		}
	}

	template<typename T>
	Array<T>::Array(const T ChFill, const SizeL Len){
		AllocNum = Len;
		Data = new T[AllocNum];
		SizeL c = 0;
		while (c < AllocNum){
			Data[c] = ChFill;
			++c;
		}
	}

	template<typename T>
	Array<T>::Array(const Array<T> &Cpy, SizeL Len){
		AllocNum = (Len < Cpy.AllocNum) ? Len : Cpy.AllocNum;
		Data = new T[AllocNum];
		SizeL c = 0;
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
	void Array<T>::Take(T *StrIn, SizeL Len) {
		Data = StrIn;
		AllocNum = Len;
	}

	template<typename T>
	void Array<T>::Give(T *&StrOut, SizeL &Len) {
		StrOut = Data;
		Len = AllocNum;
		Data = 0;
		AllocNum = 0;
	}

	template<typename T>
	void Array<T>::Swap(Array<T> &Other){
		T *TmpData = Data;
		SizeL TmpLen = AllocNum;
		AllocNum = Other.AllocNum;
		Data = Other.Data;
		Other.AllocNum = TmpLen;
		Other.Data = TmpData;
	}
	template<typename T>
	void Array<T>::SetLength(SizeL Len){
		if (AllocNum == Len) return;
		T *NewData = new T[Len];
		for (SizeL c = 0; (c < Len) && (c < AllocNum); ++c){
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
		SizeL c = 0;
		while (c < AllocNum){
			NewData[c] = (T &&)Data[c];
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
	void Array<T>::AddMissing(const Array<T> &Other, SizeL Until){
		if (Until <= AllocNum) return;
		Until = (Until < Other.AllocNum) ? Until : Other.AllocNum;
		T * NewData = new T[Until];
		SizeL c = 0;
		while (c < AllocNum){
			NewData[c] = (T &&)Data[c];
			++c;
		}
		AllocNum = Until;
		while (c < AllocNum){
			NewData[c] = Other.Data[c];
			++c;
		}
		delete[] Data;
		Data = NewData;
	}
	template<typename T>
	void Array<T>::RemBeg(SizeL NumRem){
		SizeL c = 0, Until = AllocNum - NumRem;
		T * NewData = new T[Until];
		Data += NumRem;
		while (c < Until){
			NewData[c] = (T &&)Data[c];
			++c;
		}
		Data -= NumRem;
		delete[] Data;
		Data = NewData;
		AllocNum = Until;
	}
	template<typename T>
	void Array<T>::AddBeg(SizeL NumAdd){
		if (NumAdd == 0) return;
		T * NewData = new T[AllocNum + NumAdd];
		SizeL c = 0;
		NewData += NumAdd;
		while (c < AllocNum){
			NewData[c] = (T &&)Data[c];
			++c;
		}
		delete[] Data;
		NewData -= NumAdd;
		AllocNum += NumAdd;
		Data = NewData;
	}
	template<typename T>
	void Array<T>::AddBeg(SizeL NumAdd, T Val){
		if (NumAdd == 0) return;
		T * NewData = new T[AllocNum + NumAdd];
		SizeL c = 0;
		NewData += NumAdd;
		while (c < AllocNum){
			NewData[c] = (T &&)Data[c];
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
	void Array<T>::WriteFromAt(const Array<T> &From, SizeL Beg, SizeL End) {
		if (End - Beg > From.AllocNum) End = Beg + From.AllocNum;
		if (End > AllocNum) End = AllocNum;
		Data += Beg;
		End -= Beg;
		for (SizeL c = 0; c < End; ++c) {
			Data[c] = From.Data[c];
		}
		Data -= Beg;
	}
	template<typename T>
	Array<T> Array<T>::SubArr(SizeL Start, SizeL Stop, SnzL Step) const {
		if (AllocNum == 0) return Array<T>();
		if ((Stop > AllocNum) && Stop != MAX_INT) Stop = AllocNum;
		if (Start > AllocNum) Start = AllocNum - 1;
		if (Step < 0)
		{
			bool StopMax = false;
			if (Start >= AllocNum) Start = AllocNum - 1;
			if (Stop == MAX_INT)
			{
				Stop = 0;
				StopMax = true;
			}
			else if (Start <= Stop) return Array<T>();
			SizeL Range = Start - Stop;
			SizeL Step0 = -Step;
			Array<T> Rtn;
			Rtn.SetLength(((Start - Stop) + (StopMax ? Step0 : Step0 - 1)) / Step0);
			SizeL c = 0;
			for (T &Ch : Rtn) {
				Ch = Data[Start - c * Step0];
				++c;
			}
			return Rtn;
		}
		else
		{
			if (Stop > AllocNum) Stop = AllocNum;
			if (Start >= Stop) return Array<T>();
			SizeL Step1 = Step + 1;
			SizeL c1 = 0;
			Array<T> Rtn;
			Rtn.SetLength((Stop - Start - 1) / Step1 + 1);
			for (SizeL c = 0; c < Stop; c += Step1, ++c1) {
				Rtn[c1] = Data[c];
			}
			return Rtn;
		}
	}

	template<typename T>
	bool Array<T>::operator==(const Array<T> &Cmp) const{
		if (AllocNum != Cmp.AllocNum) return false;
		SizeL c = 0;
		while (c < AllocNum){
			if (Data[c] != Cmp.Data[c]) return false;
			++c;
		}
		return true;
	}

	template<typename T>
	bool Array<T>::operator!=(const Array<T> &Cmp) const{
		if (AllocNum != Cmp.AllocNum) return true;
		SizeL c = 0;
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
		SizeL c = 0;
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
		SizeL c = 0;
		while (c < AllocNum){
			Rtn.Data[c] = Data[c];
			++c;
		}
		Rtn.Data[c] = Add;
		return Rtn;
	}

	template<typename T>
	T Array<T>::operator[](const SizeL Pos) const{
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
		SizeL c = 0;
		while (c < AllocNum){
			Data[c] = Cpy.Data[c];
			++c;
		}
		return (*this);
	}

	template<typename T>
	Array<T> &Array<T>::operator+=(const Array<T> &Add){
		T *NewData = new T[AllocNum + Add.AllocNum];
		SizeL c = 0;
		while (c < AllocNum){
			NewData[c] = (T &&)Data[c];
			++c;
		}
		if (AllocNum > 0) delete[] Data;
		NewData += AllocNum;
		c = 0;
		while (c < Add.AllocNum){
			NewData[c] = Add.Data[c];
			++c;
		}
		NewData -= AllocNum;
		AllocNum += Add.AllocNum;
		Data = NewData;
		return (*this);
	}

	template<typename T>
	Array<T> &Array<T>::operator+=(const T &Add){
		T *NewData = new T[AllocNum + 1];
		SizeL c = 0;
		while (c < AllocNum){
			NewData[c] = (T &&)Data[c];
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
	bool Array<T>::Insert(SizeL Pos, T Val){
		if (Pos > AllocNum) return false;
		T *NewData = new T[AllocNum + 1];
		++AllocNum;
		SizeL c = 0;
		while (c < Pos){
			NewData[c] = (T &&)Data[c];
			++c;
		}
		NewData[c] = Val;
		++c;
		while (c < AllocNum){
			NewData[c] = (T &&)Data[c - 1];
			++c;
		}
		delete[] Data;
		Data = NewData;
		return true;
	}

	template<typename T>
	bool Array<T>::Remove(SizeL Pos){
		if (Pos >= AllocNum) return false;
		T *NewData = new T[AllocNum - 1];
		--AllocNum;
		SizeL c = 0;
		while (c < Pos){
			NewData[c] = (T &&)Data[c];
			++c;
		}
		while (c < AllocNum){
			NewData[c] = (T &&)Data[c + 1];
			++c;
		}
		delete[] Data;
		Data = NewData;
		return true;
	}

	template<typename T>
	T &Array<T>::operator[](const SizeL Pos){
		return Data[Pos];
	}

	template<typename T>
	bool Array<T>::Contains(const T &Val) const {
		for (SizeL c = 0; c < AllocNum; ++c) if (Data[c] == Val) return true;
		return false;
	}

	template<typename T>
	bool Array<T>::Find(SizeL &Pos, T Val, bool PosIsStart){
		for (SizeL c = PosIsStart ? Pos : 0; c < AllocNum; ++c){
			if (Data[c] == Val)
			{
				Pos = c;
				return true;
			}
		}
		return false;
	}
	template<typename T>
	bool Array<T>::RFind(SizeL &Pos, T Val, bool PosIsStart){
		--Data;//to shift Data so that (Old)Data[0] is the same as (New)Data[1]
		SizeL Until = PosIsStart ? Pos : 0;
		for (SizeL c = AllocNum; c > Until; --c){
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
	SizeL Array<T>::Length() const{
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
	Array<T> &Array<T>::operator*=(SizeL Num){
		T *NewData = new T[Num * AllocNum];
		SizeL c = 0;
		while (c < Num){
			SizeL c1 = 0;
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
	Array<T> Array<T>::operator*(SizeL Num) const{
		Array<T> Rtn;
		Rtn.Data = new T[Num * AllocNum];
		SizeL c = 0;
		while (c < Num){
			SizeL c1 = 0;
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
	IterArray<T>::IterArray(Array<T> *Iter, SizeL Index){
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
	template<typename T>
	SizeL BinarySearch(T *List, SizeL Len, T &Val) {
		SizeL CurLen = Len;
		SizeL Pos = Len >> 1;
		SizeL CurPos = Len >> 1;
		while (CurLen > 0) {
			if (Val == List[Pos]) return Pos;
			else if (Val < List[Pos])//Less
			{
				CurLen >>= 1;
				Pos -= (CurLen + 1) >> 1;
			}
			else//Greater
			{
				CurLen = (CurLen - 1) >> 1;
				Pos += (CurLen >> 1) + 1;
			}
		}
		return Len;
	}
	template<typename T>
	SizeL BinaryApprox(T *List, SizeL Len, T &Val) {
		SizeL CurLen = Len;
		SizeL Pos = Len >> 1;
		SizeL CurPos = Len >> 1;
		while (CurLen > 0) {
			if (Val <= List[Pos] && (Pos == 0 || Val >= List[Pos - 1])) return Pos;
			else if (Val < List[Pos])//Less
			{
				CurLen >>= 1;
				Pos -= (CurLen + 1) >> 1;
			}
			else//Greater
			{
				CurLen = (CurLen - 1) >> 1;
				Pos += (CurLen >> 1) + 1;
			}
		}
		return Len;
	}
	template<typename T1, typename T2>
	class HashMap {
	public:
		typedef SizeL(*HashFunc)(const T1 &, SizeL);
		struct DataPair {
			T1 Key;
			T2 Val;
			DataPair *Next;
			bool IsValid;
			DataPair();
			DataPair(const T1 &KeyVal, const T2 &Value, DataPair *next = 0);
		};
	private:
		HashFunc Hasher;
		DataPair *Datas;
		SizeL AllocNum;
		//returns false if the key holder is in the root list: pointer points to it, returns true if not: pointer points to previous key holder in linked list
		bool GetWithKey(const T1 &KeyVal, DataPair *&PtrPrev);
		void CleanUp(Array<DataPair *> *Pairs = 0);
	public:
		HashMap();
		HashMap(HashMap &&Other);
		HashMap(const HashMap &Other);
		HashMap &operator=(const HashMap &Other);
		HashMap &operator=(HashMap &&Other);
		void SetHashFunc(HashFunc hf, bool ReHash = true);
		T2 &AtKey(const T1 &KeyVal);
		const T2 Get(const T1 &KeyVal) const;
		bool HasKey(const T1 &KeyVal) const;
		T2 *GetPtrVal(T1 KeyVal);
		void Put(const T1 &KeyVal, const T2 &Value);
		bool Rem(const T1 &KeyVal);
		const T2 operator[](const T1 KeyVal) const;
		T2 &operator[](const T1 KeyVal);
		void PreAlloc(SizeL Num);
		SizeL NumHashes();
		const T1 *FindKey(const T2 &Value) const;
		Array<DataPair *> GetKeyPairs() const;
		~HashMap();
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
	HashMap<T1, T2>::HashMap() {
		Hasher = 0;
		AllocNum = 8;
		Datas = new DataPair[AllocNum];
	}
	template<typename T1, typename T2>
	void HashMap<T1, T2>::SetHashFunc(typename HashMap<T1, T2>::HashFunc hf, bool ReHash) {
		Hasher = hf;
		if (ReHash) PreAlloc(AllocNum);
	}
	template<typename T1, typename T2>
	HashMap<T1, T2>::HashMap(HashMap<T1, T2> &&Other) {
		Datas = Other.Datas;
		AllocNum = Other.AllocNum;
		Hasher = Other.Hasher;
		Other.Datas = 0;
		Other.AllocNum = 0
	}
	template<typename T1, typename T2>
	HashMap<T1, T2>::HashMap(const HashMap<T1, T2> &Other) {
		Hasher = Other.Hasher;
		AllocNum = Other.AllocNum;
		Datas = new DataPair[AllocNum];
		Array<DataPair *> OrigPairs = Other.GetKeyPairs();
		for (const DataPair *Pair : OrigPairs) {
			SizeL Pos = Hasher(Pair->Key, AllocNum);
			DataPair *NewPair = &Datas[Pos];
			if (Datas[Pos].IsValid)
			{
				while (NewPair->Next != 0) NewPair = NewPair->Next;
				NewPair->Next = new DataPair();
				NewPair = NewPair->Next;
			}
			NewPair->Key = Pair->Key;
			NewPair->Val = Pair->Val;
			NewPair->IsValid = true;
		}
	}
	template<typename T1, typename T2>
	HashMap<T1, T2> &HashMap<T1, T2>::operator=(const HashMap<T1, T2> &Other) {
		CleanUp();
		Hasher = Other.Hasher;
		AllocNum = Other.AllocNum;
		Datas = new DataPair[AllocNum];
		Array<DataPair *> OrigPairs = Other.GetKeyPairs();
		for (const DataPair *Pair : OrigPairs) {
			SizeL Pos = Hasher(Pair->Key, AllocNum);
			DataPair *NewPair = &Datas[Pos];
			if (Datas[Pos].IsValid)
			{
				while (NewPair->Next != 0) NewPair = NewPair->Next;
				NewPair->Next = new DataPair();
				NewPair = NewPair->Next;
			}
			NewPair->Key = Pair->Key;
			NewPair->Val = Pair->Val;
			NewPair->IsValid = true;
		}
		return *this;
	}
	template<typename T1, typename T2>
	HashMap<T1, T2> &HashMap<T1, T2>::operator=(HashMap<T1, T2> &&Other) {
		CleanUp();
		Datas = Other.Datas;
		AllocNum = Other.AllocNum;
		Other.Datas = 0;
		Other.AllocNum = 0;
		return *this;
	}
	template<typename T1, typename T2>
	void HashMap<T1, T2>::CleanUp(Array<typename HashMap<T1, T2>::DataPair *> *Pairs) {
		if (AllocNum == 0) return;
		bool IsDelEnd = Pairs == 0;
		if (IsDelEnd) Pairs = new Array<DataPair *>((Array<DataPair *> &&)GetKeyPairs());
		//deallocate the pairs that not directly in the array of pairs
		SizeL c = Pairs->Length();
		while (c > 0)
		{
			--c;
			if ((SizeL)((*Pairs)[c] - Datas) >= AllocNum)
			{
				delete (*Pairs)[c];
			}
		}
		//deallocate the array of pairs
		delete[] Datas;
		if (IsDelEnd) delete Pairs;
	}
	template<typename T1, typename T2>
	HashMap<T1, T2>::~HashMap() {
		CleanUp();
	}
	template<typename T1, typename T2>
	T2 *HashMap<T1, T2>::GetPtrVal(T1 KeyVal) {
		DataPair *GetPair = 0;
		bool Tmp = GetWithKey(KeyVal, GetPair);
		if (GetPair == 0) return 0;
		if (Tmp) GetPair = GetPair->Next;
		return &GetPair->Val;
	}
	template<typename T1, typename T2>
	T2 &HashMap<T1, T2>::AtKey(const T1 &KeyVal) {
		DataPair *GetPair = 0;
		bool Tmp = GetWithKey(KeyVal, GetPair);
		if (GetPair == 0)
		{
			T2 TmpVal;
			Put(KeyVal, TmpVal);
			Tmp = GetWithKey(KeyVal, GetPair);
		}
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
	bool HashMap<T1, T2>::HasKey(const T1 &KeyVal) const {
		SizeL Pos = Hasher(KeyVal, AllocNum);
		if (!Datas[Pos].IsValid) return false;
		else
		{
			DataPair *GetPair = &Datas[Pos];
			if (GetPair->Key == KeyVal) return true;
			while ((GetPair = GetPair->Next) != 0) {
				if (GetPair->Key == KeyVal) return true;
			}
			return false;
		}
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
			SizeL Pos = Hasher(KeyVal, AllocNum);
			GetPair = &Datas[Pos];
			if (!Datas[Pos].IsValid)
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
		if (GetPair == 0) return false;
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
				GetPair->Key = (T1 &&)OrigNext->Key;
				GetPair->Val = (T2 &&)OrigNext->Val;
				//since we MOVED from OrigNext, deleting OrigNext shouldn't invalidate GetPair's copies of Key and Val
				delete OrigNext;
			}
		}
		return true;
	}
	template<typename T1, typename T2>
	const T2 HashMap<T1, T2>::operator[](const T1 KeyVal) const {
		return this->Get(KeyVal);
	}
	template<typename T1, typename T2>
	T2 &HashMap<T1, T2>::operator[](const T1 KeyVal) {
		return this->AtKey(KeyVal);
	}
	template<typename T1, typename T2>
	void HashMap<T1, T2>::PreAlloc(SizeL Num) {
		//basically this is rehashing the entire map with a different number of hashes
		DataPair *NewDatas = new DataPair[Num];
		Array<DataPair *> OrigPairs = (Array<DataPair *>)GetKeyPairs();
		for (DataPair *&Pair : OrigPairs) {
			SizeL Pos = Hasher(Pair->Key, Num);
			DataPair *NewPair = &NewDatas[Pos];
			if (NewDatas[Pos].IsValid)
			{
				while (NewPair->Next != 0) NewPair = NewPair->Next;
				NewPair->Next = new DataPair();
				NewPair = NewPair->Next;
			}
			NewPair->Key = (T1 &&)Pair->Key;
			NewPair->Val = (T2 &&)Pair->Val;
			NewPair->IsValid = true;
		}
		CleanUp(&OrigPairs);
		//set the array of pairs to the new array
		AllocNum = Num;
		Datas = NewDatas;
	}
	template<typename T1, typename T2>
	SizeL HashMap<T1, T2>::NumHashes() {
		SizeL Rtn = 0;
		for (SizeL c = 0; c < AllocNum; ++c) if (Datas[c].IsValid) ++Rtn;
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
		SizeL NumDats = 0;
		Array<DataPair *> DataPairs;
		for (SizeL c = 0; c < AllocNum; ++c) {
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
		for (SizeL c = 0; c < AllocNum; ++c) {
			if (!Datas[c].IsValid) continue;
			DataPair *Cur = &Datas[c];
			*IterPairs = Cur;
			++IterPairs;
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
		SizeL Pos = Hasher(KeyVal, AllocNum);
		PtrPrev = 0;
		if (!Datas[Pos].IsValid) return false;
		PtrPrev = &Datas[Pos];
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