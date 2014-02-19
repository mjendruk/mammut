#pragma once

#include <QString>

#include <glow/Program.h>


template<typename T>
void SimplePostProcPass::setUniform(const QString & name, const T & value) 
{
    m_quad->program()->setUniform(name.toStdString(), value);
}
