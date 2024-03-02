/**
 * @file ElidedLabel.cpp
 * @brief Label that elides text when it doesn't fit in the widget.
 * @date Mar 2, 2024
 *
 * Copyright 2021-2024 Mattermost-QT Developers
 *
 * This file is part of Mattermost-QT.
 *
 * Mattermost-QT is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * Mattermost-QT is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with Mattermost-QT. if not, see https://www.gnu.org/licenses/.
 */

#include "elidedlabel.h"

#include <QPaintEvent>
#include <QResizeEvent>

ElidedLabel::ElidedLabel(const QString &text, QWidget *parent, Qt::WindowFlags f)
: QLabel(text, parent, f)
{
	setElideMode(Qt::ElideRight);
}

void ElidedLabel::setElideMode(Qt::TextElideMode elideMode)
{
	m_elideMode = elideMode;
	m_cachedText.clear();
	update();
}

void ElidedLabel::resizeEvent(QResizeEvent *e)
{
	QLabel::resizeEvent(e);
	m_cachedText.clear();
}

QSize ElidedLabel::minimumSizeHint() const
{
	const QFontMetrics fm(fontMetrics());
	return fm.size(Qt::TextSingleLine, "Some short text");
}

QSize ElidedLabel::sizeHint() const
{
	const QFontMetrics fm(fontMetrics());
	const QSize textSize = fm.size(Qt::TextSingleLine, text());
	const QSize parentSize = parentWidget()->size();
	const QPoint ourPosition = pos();

	const int freeSpace = parentSize.width() - ourPosition.x();
	QSize result = textSize;
	if (freeSpace < textSize.width()) {
		result.setWidth(freeSpace);
	}

	return result;
}

void ElidedLabel::paintEvent(QPaintEvent *e)
{
	if (m_elideMode == Qt::ElideNone)
		return QLabel::paintEvent(e);

	updateCachedTexts();
	QLabel::setText(m_cachedElidedText);
	QLabel::paintEvent(e);
	QLabel::setText(m_cachedText);
}

void ElidedLabel::updateCachedTexts()
{
	const auto fullText = text();
	if (m_cachedText == fullText)
		return;
	m_cachedText = fullText;
	const QFontMetrics fm(fontMetrics());
	m_cachedElidedText = fm.elidedText(text(),
	                                   m_elideMode,
	                                   width(),
	                                   Qt::TextShowMnemonic);

	// make sure to show at least the first character
	if (!m_cachedText.isEmpty())
	{
		const QString showFirstCharacter = m_cachedText.at(0) + QStringLiteral("...");
		setMinimumWidth(fm.horizontalAdvance(showFirstCharacter) + 1);
		setToolTip(fullText);
	}
}
