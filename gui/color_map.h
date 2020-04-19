#pragma once

#include <QtCore/QtCore>

#include "core/transaction.h"

namespace gui {

const std::map<finance::Transaction::Category, Qt::GlobalColor> kColorMap{
    {finance::Transaction::Category::Cash, Qt::GlobalColor::blue},
    {finance::Transaction::Category::Essentials, Qt::GlobalColor::darkYellow},
    {finance::Transaction::Category::Income, Qt::GlobalColor::darkGreen},
    {finance::Transaction::Category::Insurance, Qt::GlobalColor::gray},
    {finance::Transaction::Category::Internal, Qt::GlobalColor::black},
    {finance::Transaction::Category::Investment, Qt::GlobalColor::green},
    {finance::Transaction::Category::Learning, Qt::GlobalColor::cyan},
    {finance::Transaction::Category::Rent, Qt::GlobalColor::darkRed},
    {finance::Transaction::Category::Sparetime, Qt::GlobalColor::darkBlue},
    {finance::Transaction::Category::Sport, Qt::GlobalColor::darkCyan},
    {finance::Transaction::Category::SupportOthers, Qt::GlobalColor::magenta},
    {finance::Transaction::Category::Unknown, Qt::GlobalColor::black}};
}