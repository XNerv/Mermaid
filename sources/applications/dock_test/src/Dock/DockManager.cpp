//=============================================================================
#include "DockManager.h"
#include "DockButtons.h"
//=============================================================================
DockManager::DockManager ()
	: top (0), left (0), center (0), right (0), bottom (0),
	  topButton (0), leftButton (0), rightButton (0), bottomButton (0),
	  lastMouseEvent (0)
{
	addAndMakeVisible (center = new DockHolder(this, 0, CENTER), ZORDER_CENTER);

	addAndMakeVisible (left = new DockHolder(this, 0, LEFT), ZORDER_LEFT);
	addAndMakeVisible (right = new DockHolder(this, 0, RIGHT), ZORDER_RIGHT);

	left->addComponentListener (this);
	right->addComponentListener (this);

	addAndMakeVisible (leftButton = getDockableButton (juce::String("left"), LEFT), ZORDER_LEFT_BUTTON);
	leftButton->setToggleState (false, false);
	leftButton->setClickingTogglesState (true);
	//leftButton->setBackgroundColours (juce::Colours::black, juce::Colours::black);
	leftButton->addMouseListener (this, true);
	leftButton->addListener (this);

	addAndMakeVisible (rightButton = getDockableButton (juce::String("right"), RIGHT), ZORDER_RIGHT_BUTTON);
	rightButton->setToggleState (false, false);
	rightButton->setClickingTogglesState (true);
	//rightButton->setBackgroundColours (juce::Colours::black, juce::Colours::black);
	rightButton->addMouseListener (this, true);
	rightButton->addListener (this);

	addAndMakeVisible (top = new DockHolder(this, 0, TOP), ZORDER_TOP);
	addAndMakeVisible (bottom = new DockHolder(this, 0, BOTTOM), ZORDER_BOTTOM);

	top->addComponentListener (this);
	bottom->addComponentListener (this);

	addAndMakeVisible (topButton = getDockableButton (juce::String("top"), TOP), ZORDER_TOP_BUTTON);
	topButton->setToggleState (false, false);
	topButton->setClickingTogglesState (true);
	//topButton->setBackgroundColours (juce::Colours::black, juce::Colours::black);
	topButton->addMouseListener (this, true);
	topButton->addListener (this);

	addAndMakeVisible (bottomButton = getDockableButton (juce::String("bottom"), BOTTOM), ZORDER_BOTTOM_BUTTON);
	bottomButton->setToggleState (false, false);
	bottomButton->setClickingTogglesState (true);
	//bottomButton->setBackgroundColours (juce::Colours::black, juce::Colours::black);
	bottomButton->addMouseListener (this, true);
	bottomButton->addListener (this);
}
//=============================================================================
DockManager::~DockManager ()
{
	deleteAllChildren ();
}
//=============================================================================
void DockManager::timerCallback ()
{
}
//=============================================================================
void DockManager::mousePropagation (const juce::MouseEvent &e, const int position)
{
	if (position != TOP)	top->mouseExit (e);
	if (position != LEFT)	left->mouseExit (e);
	if (position != RIGHT)	right->mouseExit (e);
	if (position != BOTTOM) bottom->mouseExit (e);
	if (position != CENTER) center->mouseExit (e);
}
//=============================================================================
bool DockManager::isDocked (const int position)
{
	switch (position)
	{
		case TOP:
			return top->isDocked ();
		case LEFT:
			return left->isDocked ();
		case RIGHT:
			return right->isDocked ();
		case BOTTOM:
			return bottom->isDocked ();
		default:
			return false;
	}
}
//=============================================================================
void DockManager::setPanelComponent (const int position, juce::Component *component)
{
	switch (position)
	{
		case CENTER:
			{
				if (center)
					delete center;
				center = new DockPanel (this, component, CENTER);
				addAndMakeVisible (center, ZORDER_CENTER);
			}
			break;
		case LEFT:
			{
				if (left)
					delete left;
				left = new DockPanel (this, component, LEFT);
				left->addComponentListener (this);
				addAndMakeVisible (left, ZORDER_LEFT);
			}
			break;
		case RIGHT:
			{
				if (right)
					delete right;
				right = new DockPanel (this, component, RIGHT);
				right->addComponentListener (this);
				addAndMakeVisible (right, ZORDER_RIGHT);
			}
			break;
		case TOP:
			{
				if (top)
					delete top;
				top = new DockPanel (this, component, TOP);
				top->addComponentListener (this);
				addAndMakeVisible (top, ZORDER_TOP);
			}
			break;
		case BOTTOM:
			{
				if (bottom)
					delete bottom;
				bottom = new DockPanel (this, component, BOTTOM);
				bottom->addComponentListener (this);
				addAndMakeVisible (bottom, ZORDER_BOTTOM);
			}
			break;
		default:
			break;
	}
}
//=============================================================================
void DockManager::paint (juce::Graphics &g)
{
	g.fillAll (juce::Colours::black);
}
//=============================================================================
void DockManager::componentMovedOrResized (juce::Component &component, bool wasMoved, bool wasResized)
{
	if (wasResized)
		resized ();
}
//=============================================================================
void DockManager::resized ()
{
	const int w = getWidth();
	const int h = getHeight();

	const int b = BUTTONSIZE;

	const int th = top->getBounds().getHeight();
	const int lw = left->getBounds().getWidth();
	const int rw = right->getBounds().getWidth();
	const int bh = bottom->getBounds().getHeight();

	const bool td = top->isDocked();
	const bool ld = left->isDocked();
	const bool rd = right->isDocked();
	const bool bd = bottom->isDocked();

	int cx, cy, cw, ch;
	cx = b + (ld ? lw : 0);
	cw = w - (ld ? lw : 0) 
		   - (rd ? rw : 0) - (b*2);
	cy = b + (td ? th : 0);
	ch = h - (td ? th : 0) 
		   - (bd ? bh : 0) - (b*2);

	top->setBounds (0, b, w, th);

	left->setBounds (b, 
					 b + (td ? th : 0), 
					 lw, 
					 h - (td ? th : 0) 
					   - (bd ? bh : 0) - (b*2));

	center->setBounds (cx, cy, cw, ch);

	right->setBounds (w - rw - b, 
					  b + (td ? th : 0), 
					  rw, 
					  h - (td ? th : 0) 
						- (bd ? bh : 0) - (b*2));

	bottom->setBounds (0, h - b - bh, w, bh);

	topButton->setBounds (0, 0, w, b);

	leftButton->setBounds (0, 
						   b + (td ? th : 0), 
						   b, 
						   h - (td ? th : 0) 
							 - (bd ? bh : 0) - (b*2));

	rightButton->setBounds (w - b, 
							b + (td ? th : 0), 
							b, 
							h - (td ? th : 0) 
							  - (bd ? bh : 0) - (b*2));

	bottomButton->setBounds (0, h - b, w, b);
}
//=============================================================================
void DockManager::buttonClicked (juce::Button *button)
{
	if (button == topButton)
	{
		top->setOpen (!button->getToggleState());
		top->setDocked (!button->getToggleState());
		top->setVisible (!button->getToggleState());
	}
	else
	if (button == leftButton)
	{
		left->setOpen (!button->getToggleState());
		left->setDocked (!button->getToggleState());
		left->setVisible (!button->getToggleState());
	}
	else
	if (button == rightButton)
	{
		right->setOpen (!button->getToggleState());
		right->setDocked (!button->getToggleState());
		right->setVisible (!button->getToggleState());
	}
	else
	if (button == bottomButton)
	{
		bottom->setOpen (!button->getToggleState());
		bottom->setDocked (!button->getToggleState());
		bottom->setVisible (!button->getToggleState());
	}
	resized ();
}
//=============================================================================
void DockManager::mouseEnter (const juce::MouseEvent &e)
{
	juce::Component *animate = 0;
	juce::Component *child = 0;
	juce::Rectangle<int> position;
	int runningMs;

	if (e.eventComponent == topButton)
	{
		if (!top->isDocked() && 
			!top->isOpen())
		{
			top->setOpen (true);
			animate = top;
			child = dynamic_cast<DockPanel*>(top)->content;
			child->setAlpha (1.0f);
			position = animate->getBounds ();
			animate->setBounds (position.getX(), 
								position.getY() - position.getHeight(),
								position.getWidth(), 
								position.getHeight());
		}
	}
	else
	if (e.eventComponent == leftButton)
	{
		if (!left->isDocked() && 
			!left->isOpen())
		{
			left->setOpen (true);
			animate = left;
			child = dynamic_cast<DockPanel*>(left)->content;
			child->setAlpha (1.0f);
			position = animate->getBounds ();
			animate->setBounds (position.getX() - position.getWidth(), 
								position.getY(),
								position.getWidth(), 
								position.getHeight());
		}
	}
	else
	if (e.eventComponent == rightButton)
	{
		if (!right->isDocked() && 
			!right->isOpen())
		{
			right->setOpen (true);
			animate = right;
			child = dynamic_cast<DockPanel*>(right)->content;
			child->setAlpha (1.0f);
			position = animate->getBounds ();
			animate->setBounds (position.getX() + position.getWidth(), 
								position.getY(),
								position.getWidth(), 
								position.getHeight());
		}
	}
	else
	if (e.eventComponent == bottomButton)
	{
		if (!bottom->isDocked() && 
			!bottom->isOpen())
		{
			bottom->setOpen (true);
			animate = bottom;
			child = dynamic_cast<DockPanel*>(bottom)->content;
			child->setAlpha (1.0f);
			position = animate->getBounds ();
			animate->setBounds (position.getX(), 
								getHeight() + position.getHeight(),
								position.getWidth(), 
								position.getHeight());
		}
	}

	if (animate)
	{
		if (!animator.isAnimating (animate) && !animate->isVisible())
		{	
			child->setVisible (true);
			animate->setVisible (true);
			runningMs= juce::roundDoubleToInt(juce::jlimit(0.0, 120.0, (position.getWidth() + position.getHeight()) * 0.25));
			animator.animateComponent (animate, position, 1.0, runningMs, true, 1.0, 0.0);
		}
	}
}

void DockManager::mouseExit (const juce::MouseEvent &e)
{
	juce::Component *leftContent = dynamic_cast<DockPanel*>(left)->content;
	juce::Component *rightContent = dynamic_cast<DockPanel*>(right)->content;

	if (e.eventComponent == leftButton && !left->isDocked())
	{
		if (e.y < leftButton->getY() ||
			e.y > leftButton->getHeight() - BUTTONSIZE)
		{
			animator.fadeOut (leftContent, FADEOUTMS);
			animator.fadeOut (left, FADEOUTMS);
			left->setOpen (false);
		}
	}
	else
	if (e.eventComponent == rightButton && !right->isDocked())
	{
		if (e.y < rightButton->getY() ||
			e.y > rightButton->getHeight() - BUTTONSIZE)
		{
			animator.fadeOut (rightContent, FADEOUTMS);
			animator.fadeOut (right, FADEOUTMS);
			right->setOpen (false);
		}
	}
}
