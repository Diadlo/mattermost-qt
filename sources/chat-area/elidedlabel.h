/**
 * @file ElidedLabel.h
 * @brief Label that elides text when it doesn't fit in the widget.
 * @note File is named lowercase to make usable in .ui files.
 * @note Based on the sample from https://wiki.qt.io/Elided_Label
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

#pragma once
#include <QLabel>

class ElidedLabel : public QLabel
{
    Q_OBJECT
public:
		explicit ElidedLabel(const QString &text, QWidget *parent=nullptr, Qt::WindowFlags f=Qt::WindowFlags());

    using QLabel::QLabel;
    // Set the elide mode used for displaying text.
    void setElideMode(Qt::TextElideMode elideMode);
    // Get the elide mode currently used to display text.
    Qt::TextElideMode elideMode() const { return m_elideMode; }

protected:
		QSize sizeHint() const override;
		QSize minimumSizeHint() const override;

    void paintEvent(QPaintEvent *e) override;
    void resizeEvent(QResizeEvent *e) override;

private:
    void updateCachedTexts();

private:
    Qt::TextElideMode m_elideMode = Qt::ElideRight;
    QString m_cachedElidedText;
    QString m_cachedText;
};
