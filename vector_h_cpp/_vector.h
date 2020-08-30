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
    void traverse(void (*)(T& V)); //������ʹ�ú���ָ�룬ֻ����ֲ����޸ģ�
    template <typename VST> 
    void traverse(VST & SO); //������ʹ�ú������󣬿�ȫ�����޸ģ�
};

#endif

