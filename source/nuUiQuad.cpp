#include "nuUiQuad.h"
#include "nuContext.h"
#include "nuGraphics.h"
#include "nuModel.h"

using namespace Nultima;

UiQuad::UiQuad(int x, int y, int width, int height, unsigned int texture) :
    m_x(x),
    m_y(y),
    m_width(width),
    m_height(height),
    m_texture(texture)
{
    Context* context = Context::get();
    m_model = context->getModel(Model::UNIT_PLANE);
}

void UiQuad::render() const
{
    Context* context = Context::get();
    Graphics* graphics = context->getGraphics();

    graphics->pushMatrix();
    graphics->translate((float)m_x, (float)m_y, 0);
    graphics->scale((float)m_width, (float)m_height, 1);
    graphics->bindTexture(m_texture);
    m_model->render();
    graphics->popMatrix();
}
