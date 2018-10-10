#include "Sample.h"

int Sample::getLength() const {
    return length;
}

float *Sample::getSamples(long startIndex) const {
    if(m_data.samples.size() < startIndex)
        return m_data.samples.begin() + startIndex;
    return nullptr;
}
