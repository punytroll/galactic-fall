#include "boxes.h"

Boxes::Box::Box(void) :
 m_pLayout(0),
 m_pParent(0)
{
}

Boxes::Box::~Box(void)
{
 if(m_pParent != 0)
 {
  m_pParent->vDeleteBox(this);
 }
}

Boxes::Size Boxes::Box::GetSize(void) const
{
 return GetMinimumSize();
}

Boxes::Size Boxes::Box::GetMinimumSize(void) const
{
 if(m_pLayout != 0)
 {
  Boxes::Size MinimumSize(m_MinimumSize);

  MinimumSize.vMaximize(m_pLayout->GetMinimumSize());

  return MinimumSize;
 }
 else
 {
  return m_MinimumSize;
 }
}

void Boxes::Box::vSetMinimumSize(const Size & MinimumSize)
{
 m_MinimumSize = MinimumSize;
 if(m_pLayout != 0)
 {
  m_pLayout->bHandleResize(m_MinimumSize);
 }
 if(m_pParent != 0)
 {
  m_pParent->vHandleResize();
 }
}

void Boxes::Box::vAddBox(Boxes::Box * pBox, const Layout::AddOptions & AddOptions)
{
 if(pBox->m_pParent != 0)
 {
  throw;
 }
 if(m_pLayout == 0)
 {
  throw;
 }
 m_pLayout->vAdd(pBox, AddOptions);
 pBox->m_pParent = this;
 vHandleResize();
}

void Boxes::Box::vDeleteBox(Boxes::Box * pBox)
{
 if(pBox->m_pParent != this)
 {
  throw;
 }
 if(m_pLayout == 0)
 {
  throw;
 }
 m_pLayout->vDelete(pBox);
 pBox->m_pParent = 0;
 vHandleResize();
}

void Boxes::Box::vAddBox(Boxes::Box * pBox)
{
 Boxes::Layout::AddOptions AddOptions(Boxes::Layout::NONE);

 vAddBox(pBox, AddOptions);
}

void Boxes::Box::vSetLayout(Layout * pLayout)
{
 if(m_pLayout != 0)
 {
  throw;
 }
 else
 {
  m_pLayout = pLayout;
 }
}

Boxes::Layout * Boxes::Box::pGetLayout(void)
{
 return m_pLayout;
}

Boxes::Box * Boxes::Box::pGetParent(void)
{
 return m_pParent;
}

void Boxes::Box::vHandleResize(void)
{
 if(m_pLayout == 0)
 {
  return;
 }
 m_pLayout->bHandleResize(m_MinimumSize);
 if(m_pParent != 0)
 {
  m_pParent->vHandleResize();
 }
}

std::ostream & Boxes::operator<<(std::ostream & OStream, const Boxes::Box & Box)
{
 return OStream << Box.GetMinimumSize();
}

Boxes::FreeLayout::AddOptions::AddOptions(float fX, float fY) :
 Boxes::Layout::AddOptions(Layout::NONE),
 m_fX(fX),
 m_fY(fY)
{
}

Boxes::Layout::ChildOptions::ChildOptions(void) :
 m_fPositionX(0),
 m_fPositionY(0)
{
}

void Boxes::FreeLayout::vAdd(Boxes::Box * pBox, const Boxes::Layout::AddOptions & AddOptions)
{
 const Boxes::FreeLayout::AddOptions * pFreeLayoutAddOptions(dynamic_cast< const Boxes::FreeLayout::AddOptions * >(&AddOptions));

 if(AddOptions.m_iMethod == Boxes::Layout::NONE)
 {
  if(pFreeLayoutAddOptions != 0)
  {
   Boxes::Layout::ChildOptions * pChildOptions(new Boxes::Layout::ChildOptions());

   pChildOptions->m_pBox = pBox;
   pChildOptions->m_fPositionX = pFreeLayoutAddOptions->m_fX;
   pChildOptions->m_fPositionY = pFreeLayoutAddOptions->m_fY;
   m_Boxes.push_back(pChildOptions);
  }
  else
  {
   Boxes::Layout::ChildOptions * pChildOptions(new Boxes::Layout::ChildOptions());

   pChildOptions->m_pBox = pBox;
   m_Boxes.push_back(pChildOptions);
  }
 }
 else
 {
  throw;
 }
}

bool Boxes::FreeLayout::bHandleResize(const Boxes::Size & BoxSize)
{
 m_MinimumSize = Boxes::Size(0.0f, 0.0f);
 for(std::deque< Box * >::size_type stBox = 0; stBox < m_Boxes.size(); ++stBox)
 {
  Boxes::Size BoxSize(m_Boxes[stBox]->m_pBox->GetSize());

  BoxSize.m_fWidth += m_Boxes[stBox]->m_fPositionX;
  BoxSize.m_fHeight += m_Boxes[stBox]->m_fPositionY;
  m_MinimumSize.vMaximize(BoxSize);
 }

 return true;
}


Boxes::HorizontalLayout::ChildOptions::ChildOptions(void)
{
}

Boxes::HorizontalLayout::ChildOptions::~ChildOptions(void)
{
}

void Boxes::HorizontalLayout::vAdd(Boxes::Box * pBox, const Boxes::HorizontalLayout::AddOptions & AddOptions)
{
 Boxes::HorizontalLayout::ChildOptions * pChildOptions(new Boxes::HorizontalLayout::ChildOptions());

 pChildOptions->m_pBox = pBox;
 pChildOptions->m_fPositionX = 0.0f;
 pChildOptions->m_Method = static_cast< Boxes::HorizontalLayout::Method >(AddOptions.m_iMethod);
 pChildOptions->m_fPositionY = 0.0f;
 m_Boxes.push_back(pChildOptions);
}

bool Boxes::HorizontalLayout::bHandleResize(const Boxes::Size & BoxSize)
{
 m_MinimumSize = Boxes::Size(0.0f, 0.0f);
 for(std::deque< Boxes::Layout::ChildOptions * >::size_type stBox = 0; stBox < m_Boxes.size(); ++stBox)
 {
  m_MinimumSize.m_fWidth += m_Boxes[stBox]->m_pBox->GetSize().m_fWidth;
  if(m_Boxes[stBox]->m_pBox->GetSize().m_fHeight > m_MinimumSize.m_fHeight)
  {
   m_MinimumSize.m_fHeight = m_Boxes[stBox]->m_pBox->GetSize().m_fHeight;
  }
 }

 float fPackBeginX(0.0f);

 for(std::deque< Boxes::Layout::ChildOptions * >::size_type stBox = 0; stBox < m_Boxes.size(); ++stBox)
 {
  if(dynamic_cast< Boxes::HorizontalLayout::ChildOptions * >(m_Boxes[stBox])->m_Method == Boxes::HorizontalLayout::PACK_BEGIN)
  {
   m_Boxes[stBox]->m_fPositionX = fPackBeginX;
   fPackBeginX += m_Boxes[stBox]->m_pBox->GetSize().m_fWidth;
  }
 }

 float fPackEndX;

 if(BoxSize.m_fWidth > m_MinimumSize.m_fWidth)
 {
  fPackEndX = BoxSize.m_fWidth;
 }
 else
 {
  fPackEndX = m_MinimumSize.m_fWidth;
 }

 for(std::deque< Boxes::Layout::ChildOptions * >::size_type stBox = 0; stBox < m_Boxes.size(); ++stBox)
 {
  if(dynamic_cast< Boxes::HorizontalLayout::ChildOptions * >(m_Boxes[stBox])->m_Method == Boxes::HorizontalLayout::PACK_END)
  {
   fPackEndX -= m_Boxes[stBox]->m_pBox->GetSize().m_fWidth;
   m_Boxes[stBox]->m_fPositionX = fPackEndX;
  }
 }

 return true;
}

Boxes::Layout::~Layout(void)
{
}

Boxes::Layout::iterator Boxes::Layout::begin(void)
{
 return Boxes::Layout::iterator(m_Boxes, 0);
}

Boxes::Layout::iterator Boxes::Layout::end(void)
{
 return Boxes::Layout::iterator(m_Boxes, m_Boxes.size());
}

void Boxes::Layout::vDelete(Boxes::Box * pBox)
{
 for(std::deque< Boxes::Layout::ChildOptions * >::iterator iBox = m_Boxes.begin(); iBox < m_Boxes.end(); ++iBox)
 {
  if((*iBox)->m_pBox == pBox)
  {
   m_Boxes.erase(iBox);

   return;
  }
 }
}

Boxes::Size Boxes::Layout::GetMinimumSize(void)
{
 return m_MinimumSize;
}

Boxes::Layout::AddOptions::AddOptions(int iMethod) :
 m_iMethod(iMethod)
{
}

Boxes::Layout::AddOptions::~AddOptions(void)
{
}

Boxes::Layout::ChildOptions::~ChildOptions(void)
{
}


Boxes::Size::Size(void) :
 m_fWidth(0.0f),
 m_fHeight(0.0f)
{
}

Boxes::Size::Size(float fWidth, float fHeight) :
 m_fWidth(fWidth),
 m_fHeight(fHeight)
{
}

Boxes::Size::Size(const Boxes::Size & OtherSize) :
 m_fWidth(OtherSize.m_fWidth),
 m_fHeight(OtherSize.m_fHeight)
{
}

void Boxes::Size::vMaximize(const Boxes::Size & OtherSize)
{
 if(OtherSize.m_fWidth > m_fWidth)
 {
  m_fWidth = OtherSize.m_fWidth;
 }
 if(OtherSize.m_fHeight > m_fHeight)
 {
  m_fHeight = OtherSize.m_fHeight;
 }
}

bool Boxes::Size::operator!=(const Boxes::Size & OtherSize) const
{
 return (m_fWidth != OtherSize.m_fWidth) || (m_fHeight != OtherSize.m_fHeight);
}

Boxes::Size Boxes::Size::operator*(float fScalar)
{
 return Boxes::Size(m_fWidth * fScalar, m_fHeight * fScalar);
}

Boxes::Size Boxes::Size::operator/(float fScalar)
{
 return Boxes::Size(m_fWidth / fScalar, m_fHeight / fScalar);
}

std::ostream & Boxes::operator<<(std::ostream & OStream, const Boxes::Size & Size)
{
 return OStream << Size.m_fWidth << 'x' << Size.m_fHeight;
}


Boxes::VerticalLayout::ChildOptions::ChildOptions(void)
{
}

Boxes::VerticalLayout::ChildOptions::~ChildOptions(void)
{
}

void Boxes::VerticalLayout::vAdd(Boxes::Box * pBox, const Boxes::Layout::AddOptions & AddOptions)
{
 Boxes::VerticalLayout::ChildOptions * pChildOptions(new Boxes::VerticalLayout::ChildOptions());

 pChildOptions->m_pBox = pBox;
 pChildOptions->m_fPositionX = 0.0f;
 pChildOptions->m_fPositionY = 0.0f;
 pChildOptions->m_Method = static_cast< Boxes::VerticalLayout::Method >(AddOptions.m_iMethod);
 m_Boxes.push_back(pChildOptions);
}

bool Boxes::VerticalLayout::bHandleResize(const Boxes::Size & BoxSize)
{
 m_MinimumSize = Boxes::Size(0.0f, 0.0f);
 for(std::deque< Boxes::Layout::ChildOptions * >::size_type stBox = 0; stBox < m_Boxes.size(); ++stBox)
 {
  m_MinimumSize.m_fHeight += m_Boxes[stBox]->m_pBox->GetSize().m_fHeight;
  if(m_Boxes[stBox]->m_fPositionX + m_Boxes[stBox]->m_pBox->GetSize().m_fWidth > m_MinimumSize.m_fWidth)
  {
   m_MinimumSize.m_fWidth = m_Boxes[stBox]->m_fPositionX + m_Boxes[stBox]->m_pBox->GetSize().m_fWidth;
  }
 }

 float fPackBeginY(0.0f);

 for(std::deque< Boxes::Layout::ChildOptions * >::size_type stBox = 0; stBox < m_Boxes.size(); ++stBox)
 {
  if(dynamic_cast< Boxes::VerticalLayout::ChildOptions * >(m_Boxes[stBox])->m_Method == Boxes::VerticalLayout::PACK_BEGIN)
  {
   m_Boxes[stBox]->m_fPositionY = fPackBeginY;
   fPackBeginY += m_Boxes[stBox]->m_pBox->GetSize().m_fHeight;
  }
 }

 float fPackEndY;

 if(BoxSize.m_fHeight > m_MinimumSize.m_fHeight)
 {
  fPackEndY = BoxSize.m_fHeight;
 }
 else
 {
  fPackEndY = m_MinimumSize.m_fHeight;
 }

 for(std::deque< Boxes::Layout::ChildOptions * >::size_type stBox = 0; stBox < m_Boxes.size(); ++stBox)
 {
  if(dynamic_cast< Boxes::VerticalLayout::ChildOptions * >(m_Boxes[stBox])->m_Method == Boxes::VerticalLayout::PACK_END)
  {
   fPackEndY -= m_Boxes[stBox]->m_pBox->GetSize().m_fHeight;
   m_Boxes[stBox]->m_fPositionY = fPackEndY;
  }
 }

 return true;
}
