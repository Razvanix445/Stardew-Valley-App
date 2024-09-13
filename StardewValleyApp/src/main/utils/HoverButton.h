#pragma once

#include <QPushButton>
#include <QEvent>
#include <QEnterEvent>

class HoverButton : public QPushButton
{
	Q_OBJECT

public:
	/**
	  * @brief A QPushButton that changes its size when hovered.
	  * @param - parent (QWidget*): The parent widget.
	  * @param - originalSize (QSize): The original size of the button.
	  * @param - hoverSize(QSize): The size of the button when hovered.
	  * @return - A QPushButton that changes its size when hovered.
	  */
	explicit HoverButton(QWidget* parent = nullptr, const QSize& originalSize = QSize(), const QSize& hoverSize = QSize()) :
		QPushButton(parent), originalSize(originalSize), hoverSize(hoverSize)
	{
		setIconSize(originalSize);
		setFixedSize(hoverSize);
	}
	~HoverButton() override {}

	bool isHovered() const {
		return underMouse();
	}

	const QSize setOriginalSize(const QSize& size) {
		originalSize = size;
		setIconSize(size);
		return size;
	}

	const QSize setHoverSize(const QSize& size) {
		hoverSize = size;
		return size;
	}

protected:
	bool event(QEvent* e) override {
		if (e->type() == QEvent::Enter) {
			setIconSize(hoverSize);
		}
		else if (e->type() == QEvent::Leave) {
			setIconSize(originalSize);
		}
		return QWidget::event(e);
	}

private:
	QSize originalSize;
	QSize hoverSize;
};