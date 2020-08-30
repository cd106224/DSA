#pragma once
#ifndef ___VECTOR_H__
#define ___VECTOR_H__
#include <cstdlib>
using Rank = int;//秩
const int DEFAULT_CAPACITY = 3;//默认的初试容量

template<typename T>
class Vector
{
protected:
    Rank    m_size;//规模
    int     m_capacity;//容量
    T*      m_elem;//数据区

protected:
    void copyFrom(T const* A, Rank lo, Rank hi);//复制数组区间A[lo, hi)
    void expand();//空间不足时扩容
    void shrink();//装填因子过小时压缩
    bool bubble(Rank lo, Rank hi); //扫描交换
    void bubbleSort(Rank lo, Rank hi); //起泡排序算法
    Rank max(Rank lo, Rank hi); //选取最大元素
    void selectionSort(Rank lo, Rank hi); //选择排序算法
    void merge(Rank lo, Rank mi, Rank hi); //归并算法
    void mergeSort(Rank lo, Rank hi); //归并排序算法
    Rank partition(Rank lo, Rank hi); //轴点构造算法
    void quickSort(Rank lo, Rank hi); //快速排序算法

public:
    //构造函数
    Vector(int c = DEFAULT_CAPACITY, int s = 0, T v = 0); //容量为c、规模为s、所有元素初始为v
    Vector(T const* A, Rank lo, Rank hi);//数组区间复制
    Vector(T const * A, Rank n);//数组整体复制
    Vector(Vector<T>const & V, Rank lo, Rank hi);//向量区间复制
    Vector(Vector<T> const & V);//向量整体复制

    //析构函数
    virtual ~Vector();//释放内部空间

    //只读访问接口
    Rank size() const;//规模
    bool empty() const;//判空
    int disordered() const; //判断向量是否已排序
    Rank find(T const & e, Rank lo, Rank hi) const; //无序向量区间查找
    Rank find(T const & e) const;//无序向量整体查找
    Rank search(T const & e, Rank lo, Rank hi) const; //有序向量区间查找(默认二分)
    Rank search(T const & e) const; //有序向量整体查找

    //可写访问接口
    T& operator[](Rank r) const;//重载下标操作符，可以类似于数组形式引用各元素
    Vector<T> & operator=(Vector<T> const & V); //重载赋值操作符，以便直接克隆向量
    int remove(Rank lo, Rank hi); //删除秩在区间[lo, hi)之内的元素
    T remove(Rank r); //删除秩为r的元素
    Rank insert(Rank r, T const & e); //插入元素
    Rank insert(T const & e);//默认作为末元素插入
    Rank push_back(T const & e);//末元素插入
    void sort(Rank lo, Rank hi); //对[lo, hi)排序(默认快排)
    void sort();//整体排序
    void unsort(Rank lo, Rank hi); //对[lo, hi)置乱
    void unsort();//整体置乱
    int deduplicate(); //无序去重
    int uniquify(); //有序去重

    //遍历
    void traverse(void(*)(T& V)); //遍历（使用函数指针，只读或局部性修改）
    template <typename VST>
    void traverse(VST & SO); //遍历（使用函数对象，可全局性修改）
};


template<typename T>//元素类型
void Vector<T>::copyFrom(T const* A, Rank lo, Rank hi)//以数组区间A[lo, hi)为蓝本复制向量
{
    m_elem = new T[m_capacity = 2 * (hi - lo)];//分配空间，规模清零
    m_size = 0;
    while (lo < hi)//A[lo, hi)内的元素逐一
    {
        m_elem[m_size++] = A[lo++];//复制至_elem[0, hi - lo)
    }
}

template<typename T>
void Vector<T>::expand()//向量空间不足时扩容
{
    if (m_size < m_capacity)
    {
        return;//尚未满员时，不必扩容
    }
    if (m_capacity < DEFAULT_CAPACITY)//不低于最小容量
    {
        m_capacity = DEFAULT_CAPACITY;
    }
    T* oldElem = m_elem;
    m_elem = new T[m_capacity <<= 1];//容量加倍
    for (int i = 0; i < m_size; ++i)
    {
        m_elem[i] = oldElem[i];//复制原向量内容（T为基本类型，或已重载赋值操作符'='）
    }
    delete[] oldElem;
}

template<typename T>
void Vector<T>::shrink()//装填因子过小时压缩向量所占空间
{
    if (m_capacity < DEFAULT_CAPACITY)//不致收缩到DEFAULT_CAPACITY以下
    {
        return;
    }
    if (m_size << 2 > m_capacity)//以25%为界
    {
        return;
    }
    T* oldElem = m_elem;
    m_elem = new T[m_capacity >>= 1];//容量减半
    for (int i = 0; i < m_size; ++i)
    {
        m_elem[i] = oldElem[i];//复制原向量内容
    }
    delete[] oldElem;//释放原空间
}

template<typename T>
bool Vector<T>::bubble(Rank lo, Rank hi)//一趟扫描交换
{
    bool sorted = true;//整体有序标志
    while (++lo < hi)//自左向右，逐一检查各对相邻元素
    {
        if (m_elem[lo - 1] > m_elem[lo])//若逆序，则
        {
            sorted = false;//意味着尚未整体有序，并需要
            swap(m_elem[lo - 1], m_elem[lo]);//通过交换使局部有序
        }
    }
    return sorted;//返回有序标志
}

template<typename T>//assert: 0 <= lo < hi <= size
void Vector<T>::bubbleSort(Rank lo, Rank hi)//向量的起泡排序
{
    while (!bubble(lo, hi--));//逐趟做扫描交换，直至全序
}

template<typename T>
Rank Vector<T>::max(Rank lo, Rank hi)//在[lo, hi]内找出最大者
{
    Rank mx = hi;
    while (lo < hi--)//逆向扫描
    {
        if (m_elem[hi] > m_elem[mx])//且严格比较
        {
            mx = hi;
        }
    }
    return mx;
}

template<typename T>
void Vector<T>::selectionSort(Rank lo, Rank hi)//向量选择排序
{
    while (lo < --hi)
    {
        swap(m_elem[max(lo, hi)], m_elem[hi]);//将[hi]与[lo, hi]中的最大者交换
    }
}

template<typename T>//有序向量的归并
void Vector<T>::merge(Rank lo, Rank mi, Rank hi)//以mi为界、各自有序的子向量[lo, mi)和[mi, hi)
{
    T* A = m_elem + lo;//合并后的向量A[0, hi - lo) = _elem[lo, hi)
    int lb = mi - lo;//前子向量B[0, lb) = _elem[lo, mi)
    T* B = new T[lb];
    for (Rank i = 0; i < lb; ++i)//复制前子向量
    {
        B[i] = A[i];
    }
    int lc = hi - mi;//后子向量C[0, lc) = _elem[mi, hi)
    T* C = m_elem + mi;
    for (Rank i = 0, j = 0, k = 0; (j < lb) || (k < lc);)//将B[j]和C[k]中的小者续至A末尾
    {
        if ((j < lb) && (!(k < lc) || (B[j] <= C[k])))
        {
            A[i++] = B[j++];
        }
        if ((k < lc) && (!(j < lb) || (C[k] < B[j])))
        {
            A[i++] = C[k++];
        }
    }
    delete[] B;
    delete[] C;
    //归并后得到完整的有序向量[lo, hi)
}

template<typename T>
void Vector<T>::mergeSort(Rank lo, Rank hi)
{
    if (hi - lo < 2)
    {
        return;//单元素区间自然有序，否则...
    }
    int mi = (lo + hi) >> 1;//以中点为界
    mergeSort(lo, mi);//分别对前、后半段排序，然后归并
    mergeSort(mi, hi);
    merge(lo, mi, hi);
}

template<typename T>
Rank Vector<T>::partition(Rank lo, Rank hi) //轴点构造算法：通过调整元素位置构造区间[lo, hi]的轴点，并返回其秩
{
    T pivot = m_elem[lo]; //以首元素为候选轴点——经以上交换
    while (lo < hi)//从向量的两端交替地向中间扫描
    {
        while ((lo < hi) && (pivot <= m_elem))//在不小于pivot的前提下
        {
            hi--;//向左拓展右端子向量
        }
        m_elem[lo] = m_elem[hi];//小于pivot者归入左侧子序列
        while ((lo < hi) && (m_elem[lo] <= pivot))//在不大于pivot的前提下
        {
            lo++;//向右拓展左端子向量
        }
        m_elem[hi] = m_elem[lo];
    }
    m_elem[lo] = pivot;//将备份的轴点记录置于前、后子向量之间
    return lo;//返回轴点的秩
}

template<typename T>
void Vector<T>::quickSort(Rank lo, Rank hi)
{
    if (hi - lo < 2)
    {
        return;//单元素区间自然有序，否则...
    }
    Rank mi = partition(lo, hi - 1);//在[lo, hi - 1]内构造轴点
    quickSort(lo, mi);//对前缀递归排序
    quickSort(mi + 1, hi);//对后缀递归排序
}

template<typename T>
Vector<T>::Vector(int c, int s, T v)
{
    m_elem = new T[m_capacity = c];
    for (m_size = 0; m_size < s; m_elem[m_size++] = v);
}

template<typename T>
Vector<T>::Vector(T const * A, Rank lo, Rank hi)
{
    copyFrom(A, lo, hi);
}

template<typename T>
Vector<T>::Vector(T const * A, Rank n)
{
    copyFrom(A, 0, n);
}

template<typename T>
Vector<T>::Vector(Vector<T> const & V, Rank lo, Rank hi)
{
    copyFrom(V.m_elem, lo, hi);
}

template<typename T>
Vector<T>::Vector(Vector<T> const & V)
{
    copyFrom(V.m_elem, 0, V.m_size);
}

template<typename T>
Vector<T>::~Vector()
{
    delete[] m_elem;
}

template<typename T>
Rank Vector<T>::size() const
{
    return m_size;
}

template<typename T>
bool Vector<T>::empty() const
{
    return !m_size;
}

template<typename T>
int Vector<T>::disordered() const
{
    int n = 0;//计数器
    for (int i = 1; i < m_size; ++i)//逐一检查_size - 1对相邻元素
    {
        if (m_elem[i - 1] > m_elem[i])
        {
            n++;//逆序则计数
        }
    }
    return n;//向量有序当且仅当n = 0
}

template<typename T>
Rank Vector<T>::find(T const & e, Rank lo, Rank hi) const
{//无序向量的顺序查找：返回最后一个元素e的位置；失败时，返回lo - 1
    while ((lo < hi--) && (e != m_elem[hi]));//从后向前，顺序查找
    return hi;//若hi < lo，则意味着失败；否则hi即命中元素的秩
}

template<typename T>
Rank Vector<T>::find(T const & e) const
{
    return find(e, 0, m_size);
}

template<typename T>
Rank Vector<T>::search(T const & e, Rank lo, Rank hi) const
{
    while (lo < hi)//每步迭代可能要做两次比较判断，有三个分支
    {
        Rank mi = (lo + hi) >> 1;//以中点为轴点
        if (e < m_elem[mi])
        {
            hi = mi;//深入前半段[lo, mi)继续查找
        }
        else if (m_elem[mi] < e)
        {
            lo = mi + 1;//深入后半段(mi, hi)继续查找
        }
        else return mi;
    }
    return -1;
}

template<typename T>
Rank Vector<T>::search(T const & e) const
{
    return(0 >= m_size) ? -1 : search(e, 0, m_size);
}

template<typename T>
T & Vector<T>::operator[](Rank r) const
{
    return m_elem[r];
}

template<typename T>
Vector<T>& Vector<T>::operator=(Vector<T> const& V)
{
    if (m_elem)
    {
        delete[] m_elem; //释放原有内容
    }
    copyFrom(V.m_elem, 0, V.size());//整体复制
    return *this;//返回当前对象的引用，以便链式赋值
}

template<typename T>
int Vector<T>::remove(Rank lo, Rank hi)//删除区间[lo, hi)
{
    if (lo == hi)
    {
        return 0;//出于效率考虑，单独处理退化情况，比如remove(0, 0)
    }
    while (hi < m_size)
    {
        m_elem[lo++] = m_elem[hi++];//[hi, _size)顺次前移hi - lo个单元
    }
    m_size = lo;//更新规模，直接丢弃尾部[lo, _size = hi)区间
    shrink();//若有必要，则缩容
    return hi - lo; //返回被删除元素的数目
}

template<typename T>
T Vector<T>::remove(Rank r)
{
    T e = m_elem[r];
    remove(r, r + 1);//调用区间删除算法，等效于对区间[r, r + 1)的删除
    return e;
}

template<typename T>
Rank Vector<T>::insert(Rank r, T const & e)
{
    expand();//若有必要，扩容
    for (int i = m_size; i > r; i--)
    {
        m_elem[i] = m_elem[i - 1];//自后向前，后继元素顺次后移一个单元
    }
    m_elem[r] = e;//置入新元素并更新容量
    m_size++;
    return r;//返回秩
}

template<typename T>
Rank Vector<T>::insert(T const & e)
{
    return insert(m_size, e);
}

template<typename T>
Rank Vector<T>::push_back(T const & e)
{
    insert(e);
}

template<typename T>
void Vector<T>::sort(Rank lo, Rank hi)
{
    quickSort(lo, hi);
}

template<typename T>
void Vector<T>::sort()
{
    sort(0, m_size);
}

template<typename T>
void Vector<T>::unsort(Rank lo, Rank hi)//等概率随机置乱向量区间[lo, hi)
{
    T* V = m_elem + lo;//将子向量_elem[lo, hi)视作另一向量V[0, hi - lo)
    for (Rank i = hi - lo; i > 0; i--)//自后向前
    {
        swap(V[i - 1], V[rand() % i]);//将V[i - 1]与V[0, i)中某一元素随机交换
    }
}

template<typename T>
void Vector<T>::unsort()
{
    unsort(0, m_size);
}

template<typename T>
int Vector<T>::deduplicate()//删除无序向量中重复元素（高效版）
{
    int oldSize = m_size;
    Rank i = 1;//从m_elem[1]开始
    while (i < m_size)//自前向后逐一考查各元素_elem[i]
    {
        (find(m_elem[i], 0, i) < 0) ?//在其前缀中寻找与之雷同者（至多一个）
            i++ : remove(i);//若无雷同则继续考查其后继，否则删除雷同者
    }
    return oldSize - m_size;//向量规模变化量，即被删除元素总数
}

template<typename T>
int Vector<T>::uniquify()//有序向量重复元素剔除算法（高效版）
{
    Rank i = 0, j = 0;//各对互异“相邻”元素的秩
    while (++j < m_size)//逐一扫描，直至末元素
    {
        if (m_elem[i] != m_elem[j])//跳过雷同者
        {
            m_elem[++i] = m_elem[j];//发现不同元素时，向前移至紧邻于前者右侧
        }
    }
    m_size = ++i;
    shrink();//直接截除尾部多余元素
    return j - i;//向量规模变化量，即被删除元素总数
}

template<typename T>
void Vector<T>::traverse(void(*visit)(T &))//利用函数指针机制的遍历
{
    for (int i = 0; i < m_size; ++i)
    {
        visit(m_elem[i]);
    }
}

template<typename T>
template <typename VST> //元素类型、操作器
void Vector<T>::traverse(VST & SO)
{
    for (int i = 0; i < m_size; ++i)//利用函数对象机制的遍历
    {
        SO(m_elem[i]);
    }
}


#endif

