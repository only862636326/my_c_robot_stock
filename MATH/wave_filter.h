#ifndef __WAVE_FILTER_H__
#define __WAVE_FILTER_H__


#include <stdio.h>
#include <stdlib.h>

typedef float TYPE_WAVE, *pTYPE_WAVE;
typedef float TYPE_WAVE_RETURN;

typedef enum
{
    MEDIAN,
    MEAN,
}TYPE_FILTER;



// ��ѡ������
void wave_sort(pTYPE_WAVE pfilter_head, pTYPE_WAVE pfilter_end);


/*
    ����һ��ֵ,�����˲����ֵ
    ���ѵõ���ֵ�������˲���ͷ
    
    �˲�����Сһ��Ϊ����
    
*/

// ��ֵ�˲�
TYPE_WAVE_RETURN wave_filter_mean  (pTYPE_WAVE pfilter_head, pTYPE_WAVE pfilter_end, TYPE_WAVE input_data);
// ����˲�
TYPE_WAVE_RETURN wave_filter_conv(pTYPE_WAVE pfilter_head, pTYPE_WAVE pfilter_end, 
                                  pTYPE_WAVE filter,     TYPE_WAVE input_data);
// ��ֵ�˲�
TYPE_WAVE_RETURN wave_filter_median(pTYPE_WAVE pfilter_head, pTYPE_WAVE pfilter_end, TYPE_WAVE input_data);




#endif

