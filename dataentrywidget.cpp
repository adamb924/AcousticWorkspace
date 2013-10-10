#include "dataentrywidget.h"

#include <QGridLayout>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QSlider>

#include <QtDebug>

DataEntryDialog::DataEntryDialog(const QStringList* f, QList<QVariant>* v, QString label, QWidget *parent = 0) :
    QDialog(parent)
{
    QGridLayout *layout = new QGridLayout;

    int offset=0;
    if(label.length()>0)
    {
	layout->addWidget(new QLabel(label),0,0,1,-1);
	offset = 1;
    }

//    values = QList<QVariant>(v);
    int i;
    for(i=0; i<v->count(); i++)
    {
	values << v->at(i);
    }

    for(i=0; i<f->length(); i++)
    {
	edits << new QLineEdit;
	if( v->length() > i )
	{
	    edits.last()->setText(v->at(i).toString());
	}
	layout->addWidget(new QLabel(f->at(i)),i+offset,0,Qt::AlignRight);
	layout->addWidget(edits.last(),i+offset,3);
    }

    QPushButton *bSaveAndClose;
    bSaveAndClose = new QPushButton(tr("Okay"));
    layout->addWidget(bSaveAndClose,i+1,0,1,4,Qt::AlignHCenter);

    connect(bSaveAndClose,SIGNAL(clicked()),this,SLOT(accept()));

    this->setLayout(layout);
}

void DataEntryDialog::accept()
{
    while(values.count() < edits.count() )
	values << "";
    for(int i=0; i<edits.count(); i++)
    {
	values.replace(i,edits.at(i)->text());
    }
    QDialog::accept();
}
