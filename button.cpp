#include "button.h"

Button::Button(QWidget *parent)
: QPushButton(parent)
{
    setDown(false);
    setFocusPolicy(Qt::NoFocus);
}

Button::~Button()
{

}

void Button::enterEvent(QEvent *event)
{
    if( isEnabled() && !m_FocusIcon.isNull() )
        setIcon(m_FocusIcon);
}

void Button::leaveEvent(QEvent *event)
{
    if( isEnabled() )
        setIcon(m_NormalIcon);
}
void Button::mousePressEvent(QMouseEvent *event)
{
    if( isEnabled() && !m_PressedIcon.isNull() )
        setIcon(m_PressedIcon);
    QPushButton::mousePressEvent(event);
    emit(clicked());
}

void Button::mouseReleaseEvent(QMouseEvent *event)
{
    if( isEnabled() && m_FocusIcon.isNull() )
        setIcon(m_FocusIcon);
    QPushButton::mouseReleaseEvent(event);
}

void Button::SetButtonIcons(const QIcon &normal,
    const QIcon &focus, const QIcon &pressed)
{
    m_NormalIcon = normal;
    m_FocusIcon = focus;
    m_PressedIcon = pressed;
  //  setFixedSize(35,35);
    setIcon(m_NormalIcon);
}
