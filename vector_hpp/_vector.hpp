#pragma once
#ifndef ___VECTOR_H__
#define ___VECTOR_H__
#include <cstdlib>
using Rank = int;//��
const int DEFAULT_CAPACITY = 3;//Ĭ�ϵĳ�������

template<typename T>
class Vector
{
protected:
    Rank    m_size;//��ģ
    int     m_capacity;//����
    T*      m_elem;//������

protected:
    void copyFrom(T const* A, Rank lo, Rank hi);//������������A[lo, hi)
    void expand();//�ռ䲻��ʱ����
    void shrink();//װ�����ӹ�Сʱѹ��
    bool bubble(Rank lo, Rank hi); //ɨ�轻��
    void bubbleSort(Rank lo, Rank hi); //���������㷨
    Rank max(Rank lo, Rank hi); //ѡȡ���Ԫ��
    void selectionSort(Rank lo, Rank hi); //ѡ�������㷨
    void merge(Rank lo, Rank mi, Rank hi); //�鲢�㷨
    void mergeSort(Rank lo, Rank hi); //�鲢�����㷨
    Rank partition(Rank lo, Rank hi); //��㹹���㷨
    void quickSort(Rank lo, Rank hi); //���������㷨

public:
    //���캯��
    Vector(int c = DEFAULT_CAPACITY, int s = 0, T v = 0); //����Ϊc����ģΪs������Ԫ�س�ʼΪv
    Vector(T const* A, Rank lo, Rank hi);//�������临��
    Vector(T const * A, Rank n);//�������帴��
    Vector(Vector<T>const & V, Rank lo, Rank hi);//�������临��
    Vector(Vector<T> const & V);//�������帴��

    //��������
    virtual ~Vector();//�ͷ��ڲ��ռ�

    //ֻ�����ʽӿ�
    Rank size() const;//��ģ
    bool empty() const;//�п�
    int disordered() const; //�ж������Ƿ�������
    Rank find(T const & e, Rank lo, Rank hi) const; //���������������
    Rank find(T const & e) const;//���������������
    Rank search(T const & e, Rank lo, Rank hi) const; //���������������(Ĭ�϶���)
    Rank search(T const & e) const; //���������������

    //��д���ʽӿ�
    T& operator[](Rank r) const;//�����±������������������������ʽ���ø�Ԫ��
    Vector<T> & operator=(Vector<T> const & V); //���ظ�ֵ���������Ա�ֱ�ӿ�¡����
    int remove(Rank lo, Rank hi); //ɾ����������[lo, hi)֮�ڵ�Ԫ��
    T remove(Rank r); //ɾ����Ϊr��Ԫ��
    Rank insert(Rank r, T const & e); //����Ԫ��
    Rank insert(T const & e);//Ĭ����ΪĩԪ�ز���
    Rank push_back(T const & e);//ĩԪ�ز���
    void sort(Rank lo, Rank hi); //��[lo, hi)����(Ĭ�Ͽ���)
    void sort();//��������
    void unsort(Rank lo, Rank hi); //��[lo, hi)����
    void unsort();//��������
    int deduplicate(); //����ȥ��
    int uniquify(); //����ȥ��

    //����
    void traverse(void(*)(T& V)); //������ʹ�ú���ָ�룬ֻ����ֲ����޸ģ�
    template <typename VST>
    void traverse(VST & SO); //������ʹ�ú������󣬿�ȫ�����޸ģ�
};


template<typename T>//Ԫ������
void Vector<T>::copyFrom(T const* A, Rank lo, Rank hi)//����������A[lo, hi)Ϊ������������
{
    m_elem = new T[m_capacity = 2 * (hi - lo)];//����ռ䣬��ģ����
    m_size = 0;
    while (lo < hi)//A[lo, hi)�ڵ�Ԫ����һ
    {
        m_elem[m_size++] = A[lo++];//������_elem[0, hi - lo)
    }
}

template<typename T>
void Vector<T>::expand()//�����ռ䲻��ʱ����
{
    if (m_size < m_capacity)
    {
        return;//��δ��Աʱ����������
    }
    if (m_capacity < DEFAULT_CAPACITY)//��������С����
    {
        m_capacity = DEFAULT_CAPACITY;
    }
    T* oldElem = m_elem;
    m_elem = new T[m_capacity <<= 1];//�����ӱ�
    for (int i = 0; i < m_size; ++i)
    {
        m_elem[i] = oldElem[i];//����ԭ�������ݣ�TΪ�������ͣ��������ظ�ֵ������'='��
    }
    delete[] oldElem;
}

template<typename T>
void Vector<T>::shrink()//װ�����ӹ�Сʱѹ��������ռ�ռ�
{
    if (m_capacity < DEFAULT_CAPACITY)//����������DEFAULT_CAPACITY����
    {
        return;
    }
    if (m_size << 2 > m_capacity)//��25%Ϊ��
    {
        return;
    }
    T* oldElem = m_elem;
    m_elem = new T[m_capacity >>= 1];//��������
    for (int i = 0; i < m_size; ++i)
    {
        m_elem[i] = oldElem[i];//����ԭ��������
    }
    delete[] oldElem;//�ͷ�ԭ�ռ�
}

template<typename T>
bool Vector<T>::bubble(Rank lo, Rank hi)//һ��ɨ�轻��
{
    bool sorted = true;//���������־
    while (++lo < hi)//�������ң���һ����������Ԫ��
    {
        if (m_elem[lo - 1] > m_elem[lo])//��������
        {
            sorted = false;//��ζ����δ�������򣬲���Ҫ
            swap(m_elem[lo - 1], m_elem[lo]);//ͨ������ʹ�ֲ�����
        }
    }
    return sorted;//���������־
}

template<typename T>//assert: 0 <= lo < hi <= size
void Vector<T>::bubbleSort(Rank lo, Rank hi)//��������������
{
    while (!bubble(lo, hi--));//������ɨ�轻����ֱ��ȫ��
}

template<typename T>
Rank Vector<T>::max(Rank lo, Rank hi)//��[lo, hi]���ҳ������
{
    Rank mx = hi;
    while (lo < hi--)//����ɨ��
    {
        if (m_elem[hi] > m_elem[mx])//���ϸ�Ƚ�
        {
            mx = hi;
        }
    }
    return mx;
}

template<typename T>
void Vector<T>::selectionSort(Rank lo, Rank hi)//����ѡ������
{
    while (lo < --hi)
    {
        swap(m_elem[max(lo, hi)], m_elem[hi]);//��[hi]��[lo, hi]�е�����߽���
    }
}

template<typename T>//���������Ĺ鲢
void Vector<T>::merge(Rank lo, Rank mi, Rank hi)//��miΪ�硢���������������[lo, mi)��[mi, hi)
{
    T* A = m_elem + lo;//�ϲ��������A[0, hi - lo) = _elem[lo, hi)
    int lb = mi - lo;//ǰ������B[0, lb) = _elem[lo, mi)
    T* B = new T[lb];
    for (Rank i = 0; i < lb; ++i)//����ǰ������
    {
        B[i] = A[i];
    }
    int lc = hi - mi;//��������C[0, lc) = _elem[mi, hi)
    T* C = m_elem + mi;
    for (Rank i = 0, j = 0, k = 0; (j < lb) || (k < lc);)//��B[j]��C[k]�е�С������Aĩβ
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
    //�鲢��õ���������������[lo, hi)
}

template<typename T>
void Vector<T>::mergeSort(Rank lo, Rank hi)
{
    if (hi - lo < 2)
    {
        return;//��Ԫ��������Ȼ���򣬷���...
    }
    int mi = (lo + hi) >> 1;//���е�Ϊ��
    mergeSort(lo, mi);//�ֱ��ǰ����������Ȼ��鲢
    mergeSort(mi, hi);
    merge(lo, mi, hi);
}

template<typename T>
Rank Vector<T>::partition(Rank lo, Rank hi) //��㹹���㷨��ͨ������Ԫ��λ�ù�������[lo, hi]����㣬����������
{
    T pivot = m_elem[lo]; //����Ԫ��Ϊ��ѡ��㡪�������Ͻ���
    while (lo < hi)//�����������˽�������м�ɨ��
    {
        while ((lo < hi) && (pivot <= m_elem))//�ڲ�С��pivot��ǰ����
        {
            hi--;//������չ�Ҷ�������
        }
        m_elem[lo] = m_elem[hi];//С��pivot�߹������������
        while ((lo < hi) && (m_elem[lo] <= pivot))//�ڲ�����pivot��ǰ����
        {
            lo++;//������չ���������
        }
        m_elem[hi] = m_elem[lo];
    }
    m_elem[lo] = pivot;//�����ݵ�����¼����ǰ����������֮��
    return lo;//����������
}

template<typename T>
void Vector<T>::quickSort(Rank lo, Rank hi)
{
    if (hi - lo < 2)
    {
        return;//��Ԫ��������Ȼ���򣬷���...
    }
    Rank mi = partition(lo, hi - 1);//��[lo, hi - 1]�ڹ������
    quickSort(lo, mi);//��ǰ׺�ݹ�����
    quickSort(mi + 1, hi);//�Ժ�׺�ݹ�����
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
    int n = 0;//������
    for (int i = 1; i < m_size; ++i)//��һ���_size - 1������Ԫ��
    {
        if (m_elem[i - 1] > m_elem[i])
        {
            n++;//���������
        }
    }
    return n;//���������ҽ���n = 0
}

template<typename T>
Rank Vector<T>::find(T const & e, Rank lo, Rank hi) const
{//����������˳����ң��������һ��Ԫ��e��λ�ã�ʧ��ʱ������lo - 1
    while ((lo < hi--) && (e != m_elem[hi]));//�Ӻ���ǰ��˳�����
    return hi;//��hi < lo������ζ��ʧ�ܣ�����hi������Ԫ�ص���
}

template<typename T>
Rank Vector<T>::find(T const & e) const
{
    return find(e, 0, m_size);
}

template<typename T>
Rank Vector<T>::search(T const & e, Rank lo, Rank hi) const
{
    while (lo < hi)//ÿ����������Ҫ�����αȽ��жϣ���������֧
    {
        Rank mi = (lo + hi) >> 1;//���е�Ϊ���
        if (e < m_elem[mi])
        {
            hi = mi;//����ǰ���[lo, mi)��������
        }
        else if (m_elem[mi] < e)
        {
            lo = mi + 1;//�������(mi, hi)��������
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
        delete[] m_elem; //�ͷ�ԭ������
    }
    copyFrom(V.m_elem, 0, V.size());//���帴��
    return *this;//���ص�ǰ��������ã��Ա���ʽ��ֵ
}

template<typename T>
int Vector<T>::remove(Rank lo, Rank hi)//ɾ������[lo, hi)
{
    if (lo == hi)
    {
        return 0;//����Ч�ʿ��ǣ����������˻����������remove(0, 0)
    }
    while (hi < m_size)
    {
        m_elem[lo++] = m_elem[hi++];//[hi, _size)˳��ǰ��hi - lo����Ԫ
    }
    m_size = lo;//���¹�ģ��ֱ�Ӷ���β��[lo, _size = hi)����
    shrink();//���б�Ҫ��������
    return hi - lo; //���ر�ɾ��Ԫ�ص���Ŀ
}

template<typename T>
T Vector<T>::remove(Rank r)
{
    T e = m_elem[r];
    remove(r, r + 1);//��������ɾ���㷨����Ч�ڶ�����[r, r + 1)��ɾ��
    return e;
}

template<typename T>
Rank Vector<T>::insert(Rank r, T const & e)
{
    expand();//���б�Ҫ������
    for (int i = m_size; i > r; i--)
    {
        m_elem[i] = m_elem[i - 1];//�Ժ���ǰ�����Ԫ��˳�κ���һ����Ԫ
    }
    m_elem[r] = e;//������Ԫ�ز���������
    m_size++;
    return r;//������
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
void Vector<T>::unsort(Rank lo, Rank hi)//�ȸ������������������[lo, hi)
{
    T* V = m_elem + lo;//��������_elem[lo, hi)������һ����V[0, hi - lo)
    for (Rank i = hi - lo; i > 0; i--)//�Ժ���ǰ
    {
        swap(V[i - 1], V[rand() % i]);//��V[i - 1]��V[0, i)��ĳһԪ���������
    }
}

template<typename T>
void Vector<T>::unsort()
{
    unsort(0, m_size);
}

template<typename T>
int Vector<T>::deduplicate()//ɾ�������������ظ�Ԫ�أ���Ч�棩
{
    int oldSize = m_size;
    Rank i = 1;//��m_elem[1]��ʼ
    while (i < m_size)//��ǰ�����һ�����Ԫ��_elem[i]
    {
        (find(m_elem[i], 0, i) < 0) ?//����ǰ׺��Ѱ����֮��ͬ�ߣ�����һ����
            i++ : remove(i);//������ͬ������������̣�����ɾ����ͬ��
    }
    return oldSize - m_size;//������ģ�仯��������ɾ��Ԫ������
}

template<typename T>
int Vector<T>::uniquify()//���������ظ�Ԫ���޳��㷨����Ч�棩
{
    Rank i = 0, j = 0;//���Ի��조���ڡ�Ԫ�ص���
    while (++j < m_size)//��һɨ�裬ֱ��ĩԪ��
    {
        if (m_elem[i] != m_elem[j])//������ͬ��
        {
            m_elem[++i] = m_elem[j];//���ֲ�ͬԪ��ʱ����ǰ����������ǰ���Ҳ�
        }
    }
    m_size = ++i;
    shrink();//ֱ�ӽس�β������Ԫ��
    return j - i;//������ģ�仯��������ɾ��Ԫ������
}

template<typename T>
void Vector<T>::traverse(void(*visit)(T &))//���ú���ָ����Ƶı���
{
    for (int i = 0; i < m_size; ++i)
    {
        visit(m_elem[i]);
    }
}

template<typename T>
template <typename VST> //Ԫ�����͡�������
void Vector<T>::traverse(VST & SO)
{
    for (int i = 0; i < m_size; ++i)//���ú���������Ƶı���
    {
        SO(m_elem[i]);
    }
}


#endif

