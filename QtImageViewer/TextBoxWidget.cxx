#include <QPainter>
#include <QPoint>
#include "TextBoxWidget.h"

// TODO: Fix indices
TextBoxTool::TextBoxTool(QtGlSliceView* parent, std::unique_ptr< TextBoxToolMetaData > metaData) : 
    parent{ parent }, indices{ 190, 300 }, points{ parent->indexToPhysicalPoint(190), parent->indexToPhysicalPoint(300) },
    metaData{ std::move(metaData) }, textEdit{ QString::fromStdString(this->metaData->defaultText), parent }
{
  textEdit.move(this->parent->width() - textEdit.sizeHint().width(), 0);
}

void TextBoxTool::paint()
{
  textEdit.show();
  // QPainter painter(this->parent);
  // painter.setPen(Qt::blue);
  // painter.drawEllipse(390, 390, 50, 50);
}

void TextBoxTool::hide()
{
  textEdit.hide();
}



TextBoxTool::~TextBoxTool() {}

bool operator< (std::unique_ptr< TextBoxToolMetaData > const& lhs, std::unique_ptr< TextBoxToolMetaData > const& rhs) {
    return lhs->sortId < rhs->sortId;
}

TextBoxMetaDataGenerator::TextBoxMetaDataGenerator(const std::string& defaultText) : defaultText(defaultText) {

}

std::unique_ptr< TextBoxToolMetaData > TextBoxMetaDataGenerator::operator()(void) {
    return std::unique_ptr< TextBoxToolMetaData >(new TextBoxToolMetaData{ this->curId++, this->defaultText });
}


TextBoxToolMetaDataFactory::TextBoxToolMetaDataFactory(std::unique_ptr< TextBoxMetaDataGenerator > generator) : generator{ std::move(generator) } { }

std::unique_ptr< TextBoxToolMetaData > TextBoxToolMetaDataFactory::getNext() {
    std::unique_ptr< TextBoxToolMetaData > ans;
    if (refunds.size() > 0) {
        ans = std::move(*(refunds.begin()));
        refunds.erase(refunds.begin());
    }
    else {
        ans = (*generator)();
    }
    return ans;
}

void TextBoxToolMetaDataFactory::refund(std::unique_ptr< TextBoxToolMetaData > textBoxMeta) {
    refunds.push_back(std::move(textBoxMeta));
    std::sort(refunds.begin(), refunds.end());
}
