#include "needleinstrument.h"







Arc::Arc(QObject *parent, PanelItem *panel) : QObject(parent){
    _parent = parent;
    _panel = panel;
}

void Arc::update(void){
    _panel->update();
}

void Arc::setMin(float value){
    if (value == _minValue) return;
    _minValue = value;
    update();
}

void Arc::setUse(bool use){

}

void Arc::setMax(float value){
    if (value == _maxValue) return;
    _maxValue = value;
    update();
}
void Arc::setInner(float value){
    if (value == _radius_inner) return;
    _radius_inner = value;
    update();
}
void Arc::setOuter(float value){
    if (value == _radius_outer) return;
    _radius_outer = value;
    update();
}

void Arc::setColor(QString colorString){

}










NeedleInstrument::NeedleInstrument(QObject *parent) :
    PanelItem(parent)
{
    _value = _zeroangle = 0;
    _zeroValue = 0;
    _maxValue = 1;
    _maxAngle = 360;
    _thickBars = _thinBars = 0;
    _numbers = 0;
    _numberScale = 1;
    numberFont.setPixelSize(20); // @todo configurable
}

void NeedleInstrument::setNumbers(float div) {
    _numbers = div;
}

void NeedleInstrument::setNumberScale(float ns) {
    _numberScale = ns;
}

void NeedleInstrument::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    static const QPoint needle[3] = {
        QPoint(6, 8),
        QPoint(-6, 8),
        QPoint(0, -95)
    };
    painter->setFont(numberFont);
    QColor needleColor(255, 255, 255);

    int side = qMin(width(), height());
    painter->setRenderHint(QPainter::Antialiasing);
    painter->save();
    painter->scale(side / 200.0, side / 200.0);
    painter->save();
    painter->translate(100, 100);
    painter->setPen(Qt::NoPen);
    painter->setBrush(needleColor);

    painter->save();
    float needleValue = qMax(qMin(_value, _maxValue), _zeroValue);

    painter->rotate(value2Angle(needleValue));
    painter->drawConvexPolygon(needle, 3);
    painter->restore();

    painter->setPen(Qt::white);
    if(!_label.isEmpty()) {
        int textwidth = painter->fontMetrics().width(_label);
        painter->drawText(-textwidth/2,-70, textwidth, 200, Qt::AlignCenter, _label);
    }
    if(_thickBars != 0) {
        for (float i = _zeroValue ; i <= _maxValue; i+=_thickBars) {
            painter->save();
            painter->rotate(value2Angle(i));
            painter->drawRect(-1, -100, 2, 14);
            painter->restore();
        }
    }
    if(_thinBars != 0) {
        for (float i = _zeroValue ; i <= _maxValue; i+=_thinBars) {
            painter->save();
            painter->rotate(value2Angle(i));
            painter->drawRect(-0.3, -100, 0.6, 8);
            painter->restore();
        }
    }
    painter->setPen(QColor(200,200,200));
    if(_numbers != 0) {
        for (float i = _zeroValue ; i <= _maxValue; i+=_numbers) {
            painter->save();
            painter->rotate(value2Angle(i));
            painter->save();
	    QString lineNumber = QString::number(i * _numberScale);
            painter->translate(0,-70);
            painter->rotate(-value2Angle(i));
            int width = painter->fontMetrics().width(lineNumber);
            int height = painter->fontMetrics().height();
            painter->drawText(-width/2,-height/2,width,height, Qt::AlignCenter,  lineNumber);
            painter->restore();
            painter->restore();
        }
    }
    painter->restore();

    painter->restore();
    PanelItem::paint(painter, option, widget);
}


void NeedleInstrument::setLabel(QString text) {
    _label = text;
}

void NeedleInstrument::setScale(float zeroAngle, float zeroValue, float maxAngle, float maxValue) {
    Q_ASSERT(maxAngle != 0);
    _zeroangle = zeroAngle;
    _zeroValue = zeroValue;
    _maxAngle = maxAngle;
    _maxValue = maxValue;
    update();
}


void NeedleInstrument::setValue(float value) {
    if(value == _value) return;
    _value = value;
    update();
}

void NeedleInstrument::setBars(float thick, float thin) {
    _thinBars = thin;
    _thickBars = thick;
}

float NeedleInstrument::value2Angle(float value) {
    return _zeroangle +  ((value - _zeroValue) / (_maxValue - _zeroValue)) * (_maxAngle - _zeroangle);
}







int NeedleInstrument::numberOfArcs(void) {
    return _arcs.count();
}

Arc * NeedleInstrument::getArc(int arcNo) {
    return _arcs[arcNo];
}


void NeedleInstrument::clearArcs(void) {
    foreach(Arc *g, _arcs) {
	_arcs.removeOne(g); // TODO: dankrusi: should this be delete later?
	delete g;
    }
}

void NeedleInstrument::addArc(Arc *arc) {
    _arcs.append(arc);
}

void NeedleInstrument::setArcMin(int arcNumber, float value) {
    if (_arcs[arcNumber]->_minValue==value) return;
    _arcs[arcNumber]->_minValue=value;
    update();
}
