#include "regressiondialog.h"

#include <QtWidgets>
#include <QtDebug>
#include <QList>

#include <gsl/gsl_combination.h>

#include "waveformdata.h"
#include "regression.h"

RegressionDialog::RegressionDialog(QList<WaveformData*> *wfd, QList<SpectrogramData*> *spd, QWidget *parent) :
    QDialog(parent)
{
    setSizeGripEnabled(true);

    QHBoxLayout *layout = new QHBoxLayout;

    QVBoxLayout *dependentLayout = new QVBoxLayout;
    QVBoxLayout *independentSimpleLayout = new QVBoxLayout;
    QVBoxLayout *independentInteractionLayout = new QVBoxLayout;
    QVBoxLayout *controlLayout = new QVBoxLayout;
    layout->addLayout(dependentLayout, 10);
    layout->addLayout(independentSimpleLayout, 10);
    layout->addLayout(independentInteractionLayout, 10);
    layout->addLayout(controlLayout, 10);

    dependentLayout->addWidget(new QLabel(tr("Dependent Variables")));
    independentSimpleLayout->addWidget(new QLabel(tr("Independent Variables (Simple)")));
    independentInteractionLayout->addWidget(new QLabel(tr("Independent Variables (Interactions)")));

    mDependentList = new QListWidget;
    mDependentSpectrogramList = new QListWidget;
    mIndependentSimpleList = new QListWidget;
    mIndependentInteractionList = new QListWidget;

    dependentLayout->addWidget(mDependentList);
    dependentLayout->addWidget(mDependentSpectrogramList);
    independentSimpleLayout->addWidget(mIndependentSimpleList);
    independentInteractionLayout->addWidget(mIndependentInteractionList);

    for(int i=0; i<wfd->count(); i++)
	mDependentList->addItem(new RegressionListItem(wfd->at(i)) );

    for(int i=0; i<spd->count(); i++)
	mDependentSpectrogramList->addItem(new RegressionSpectrogramListItem(spd->at(i)) );

    for(int i=0; i<wfd->count(); i++)
	mIndependentSimpleList->addItem(new RegressionListItem(wfd->at(i)) );

    QPushButton *int2xButton = new QPushButton(tr("Add 2x interactions"));
    QPushButton *int3xButton = new QPushButton(tr("Add 2x and 3x interactions"));
    QPushButton *intAllButton = new QPushButton(tr("Add all interactions"));
    QPushButton *int2xSpecificButton = new QPushButton(tr("Add 2x interaction..."));

    mInterceptTerm = new QCheckBox(tr("Include intercept term"));
    mInterceptTerm->setChecked(true);

    independentInteractionLayout->addWidget(int2xButton);
    independentInteractionLayout->addWidget(int3xButton);
    independentInteractionLayout->addWidget(intAllButton);
    independentInteractionLayout->addWidget(int2xSpecificButton);
    independentInteractionLayout->addWidget(new QLabel("<hr>"));
    independentInteractionLayout->addWidget(mInterceptTerm);

    connect(int2xButton,SIGNAL(clicked()),this,SLOT(int2x()));
    connect(int3xButton,SIGNAL(clicked()),this,SLOT(int3x()));
    connect(intAllButton,SIGNAL(clicked()),this,SLOT(intAll()));
    connect(int2xSpecificButton,SIGNAL(clicked()),this,SLOT(int2xSpecific()));

    connect(mIndependentSimpleList,SIGNAL(itemChanged(QListWidgetItem*)),this,SLOT(independentSimpleChanged(QListWidgetItem*)));
    connect(mDependentList,SIGNAL(itemChanged(QListWidgetItem*)),this,SLOT(dependentChanged(QListWidgetItem*)));

    connect(mDependentList,SIGNAL(itemChanged(QListWidgetItem*)),this,SLOT(dependentChanged(QListWidgetItem*)));
    connect(mDependentSpectrogramList,SIGNAL(itemChanged(QListWidgetItem*)),this,SLOT(dependentSpectrogramChanged(QListWidgetItem*)));

    QPushButton *saveRegression = new QPushButton(tr("Save Model"));
    saveRegression->setDefault(true);
    QPushButton *createRegression = new QPushButton(tr("Show Regression Results"));
    QPushButton *createRDataCode = new QPushButton(tr("Create R data and code"));

    mNameEdit = new QLineEdit(tr("Default name"));

    controlLayout->addWidget(new QLabel(tr("Name")));
    controlLayout->addWidget(new QLabel(tr("(if you're saving the regression)")));
    controlLayout->addWidget(mNameEdit);
    controlLayout->addWidget(saveRegression);
    controlLayout->addStretch(100);
    QLabel *l = new QLabel(tr("If you wish to save the model for later use, you must click \"%1\". If you don't have further plans for this regression, or if you've already saved it, you can use either of the buttons below to close this dialog and get whatever result you request.").arg(tr("Save Model")));
    l->setWordWrap(true);
    controlLayout->addWidget(l);
    controlLayout->addWidget(createRegression);
    controlLayout->addWidget(createRDataCode);

    connect(createRegression,SIGNAL(clicked()),this,SLOT(calculateRegression()));
    connect(createRDataCode,SIGNAL(clicked()),this,SLOT(createRDataCode()));
    connect(saveRegression,SIGNAL(clicked()),this,SLOT(saveRegression()));

    setLayout(layout);
}

QList<RegressionListItem*> RegressionDialog::checkedDependent(RegressionListItem *excluding = 0)
{
    QList<RegressionListItem*> list;
    for(int i=0; i<mDependentList->count(); i++)
	if(mDependentList->item(i)->checkState() == Qt::Checked && excluding != mDependentList->item(i))
        list << dynamic_cast<RegressionListItem*>(mDependentList->item(i));
    return list;
}


QList<RegressionListItem*> RegressionDialog::checkedSimple(RegressionListItem *excluding = 0)
{
    QList<RegressionListItem*> list;
    for(int i=0; i<mIndependentSimpleList->count(); i++)
	if(mIndependentSimpleList->item(i)->checkState() == Qt::Checked && excluding != mIndependentSimpleList->item(i))
        list << dynamic_cast<RegressionListItem*>(mIndependentSimpleList->item(i));
    return list;
}

QList<RegressionInteractionListItem*> RegressionDialog::checkedInteraction(RegressionListItem *excluding = 0)
{
    QList<RegressionInteractionListItem*> list;
    for(int i=0; i<mIndependentInteractionList->count(); i++)
	if(mIndependentInteractionList->item(i)->checkState() == Qt::Checked && excluding != mIndependentInteractionList->item(i))
        list << dynamic_cast<RegressionInteractionListItem*>(mIndependentInteractionList->item(i));
    return list;
}


void RegressionDialog::int2x()
{
    QList<RegressionListItem*> list = checkedSimple();
    addNWayInteractions(&list,2);
}

void RegressionDialog::int3x()
{
    QList<RegressionListItem*> list = checkedSimple();
    addNWayInteractions(&list,2);
    addNWayInteractions(&list,3);
}

void RegressionDialog::addNWayInteractions(QList<RegressionListItem*> *list, int n)
{
    if(list->count() < 2) { return; }
    gsl_combination *c = gsl_combination_calloc(list->count(), n);
    do
    {
	InteractionEffect *interaction = new InteractionEffect;
	for(int j=0; j<n; j++)
	    interaction->members << list->at( *(gsl_combination_data(c)+j) )->data();

	RegressionInteractionListItem *tmp = new RegressionInteractionListItem(interaction);
	tmp->setCheckState(Qt::Checked);
	mIndependentInteractionList->addItem( tmp );
    }
    while (gsl_combination_next(c) == GSL_SUCCESS);
    gsl_combination_free(c);
}

void RegressionDialog::intAll()
{
    QList<RegressionListItem*> list = checkedSimple();
    for(int i=2; i<=list.count(); i++) // say it's 5, then we want 2, 3, 4, 5
	addNWayInteractions(&list,i);
}

void RegressionDialog::int2xSpecific()
{
    QList<RegressionListItem*> list = checkedSimple();
    QStringList names;
    for(int i=0; i<list.count(); i++)
	names << list.at(i)->data()->name();

    bool ok;
    QString item = QInputDialog::getItem(this, tr("Add an interaction with..."),
					 tr("Waveform"), names, 0, false, &ok);
    int index;
    if (ok && !item.isEmpty() && (index=names.indexOf(item))!=-1)
    {
	for(int i=0; i< list.count(); i++)
	{
	    if(i!=index)
	    {
		InteractionEffect *interaction = new InteractionEffect;
		interaction->members << list.at( index )->data() << list.at( i )->data();

		RegressionInteractionListItem *tmp = new RegressionInteractionListItem(interaction);
		tmp->setCheckState(Qt::Checked);
		mIndependentInteractionList->addItem( tmp );
	    }
	}
    }
}

void RegressionDialog::dependentChanged(QListWidgetItem* item)
{
    int count=0;
    for(int i=0; i<mDependentList->count(); i++)
	if( mDependentList->item(i)->checkState() == Qt::Checked )
	    count++;

    if(count==0)
	mDependentSpectrogramList->setEnabled(true);
    else
	mDependentSpectrogramList->setEnabled(false);


    if(item->checkState()==Qt::Unchecked) {return; }

    QList<RegressionListItem*> list = checkedDependent();
    for(int i=0; i<list.count(); i++)
    {
    if( ! (list.at(i)->data()->checkCongruentWith(  dynamic_cast<RegressionListItem*>(item)->data()) ) )
	{
	    QMessageBox::critical(this,tr("Error"),tr("The times associated with ") + item->text() + tr(" are inconsistent with the other selected dependent variables (e.g., ")+list.at(i)->text()+tr("). You can create separate regression models for those models."));
	    item->setCheckState(Qt::Unchecked);
	    return;
	}
    }
}

void RegressionDialog::dependentSpectrogramChanged(QListWidgetItem* item)
{
    if(item->checkState()==Qt::Unchecked)
    {
	mDependentList->setEnabled(true);
	return;
    }
    mDependentList->setEnabled(false);

    for(int i=0; i<mDependentSpectrogramList->count(); i++)
	if( mDependentSpectrogramList->item(i) != item )
	    mDependentSpectrogramList->item(i)->setCheckState(Qt::Unchecked);
}


void RegressionDialog::independentSimpleChanged(QListWidgetItem* item)
{
    if(item->checkState()==Qt::Unchecked) { return; }

    QList<RegressionListItem*> list = checkedSimple();
    QList<RegressionListItem*> dependent = checkedDependent();
    for(int i=0; i<list.count(); i++)
    {
	for(int j=0; j<dependent.count(); j++)
	{
        if( ! (dependent.at(j)->data()->checkCongruentWith(dynamic_cast<RegressionListItem*>(item)->data()) ) )
	    {
		QMessageBox::critical(this,tr("Error"),tr("The times associated with ") + item->text() + tr(" are inconsistent with the dependent variable(s) you've selected. If you want to include this effect, you need to deselect the dependent variables that are inconsistent with it (e.g., ")+dependent.at(i)->text()+tr(")."));
		item->setCheckState(Qt::Unchecked);
		return;
	    }
	}
    if( ! (list.at(i)->data()->checkCongruentWith(dynamic_cast<RegressionListItem*>(item)->data()) ) )
	{
	    QMessageBox::critical(this,tr("Error"),tr("The times associated with ") + item->text() + tr(" are inconsistent with the other selected simple effects. If you want to include this effect, you need to deselect the effects that are inconsistent with it (e.g., ")+list.at(i)->text()+tr(")."));
	    item->setCheckState(Qt::Unchecked);
	    return;
	}
    }
}

void RegressionDialog::calculateRegression()
{
    QList<RegressionListItem*> simple = checkedSimple();
    QList<RegressionInteractionListItem*> interaction = checkedInteraction();

    RegressionModel *r = new RegressionModel;

    r->setInterceptTerm(mInterceptTerm->isChecked());

    QList<RegressionListItem*> dependent = checkedDependent();
    if(dependent.count() != 0)
    {
	r->addDependentVariables(dependent);
    }
    else
    {
	for(int i=0; i<mDependentSpectrogramList->count(); i++)
	{
	    if( mDependentSpectrogramList->item(i)->checkState() == Qt::Checked )

	    {
        r->setDependentSpectrogram(dynamic_cast<RegressionSpectrogramListItem*>(mDependentSpectrogramList->item(i))->data());
		break;
	    }
	}
    }
    r->addIndependentVariables(simple,interaction);

    r->fit();

    delete r;

    close();
}

void RegressionDialog::createRDataCode()
{
    QList<RegressionListItem*> simple = checkedSimple();
    QList<RegressionInteractionListItem*> interaction = checkedInteraction();

    RegressionModel *r = new RegressionModel;

    r->setInterceptTerm(mInterceptTerm->isChecked());

    QList<RegressionListItem*> dependent = checkedDependent();
    if(dependent.count() != 0)
    {
	r->addDependentVariables(dependent);
    }
    else
    {
	for(int i=0; i<mDependentSpectrogramList->count(); i++)
	{
	    if( mDependentSpectrogramList->item(i)->checkState() == Qt::Checked )
	    {
        r->setDependentSpectrogram(dynamic_cast<RegressionSpectrogramListItem*>(mDependentSpectrogramList->item(i))->data());
		break;
	    }
	}
    }
    r->addIndependentVariables(simple,interaction);

    r->R();

    delete r;

    close();
}

void RegressionDialog::saveRegression()
{
    QList<RegressionListItem*> simple = checkedSimple();
    QList<RegressionInteractionListItem*> interaction = checkedInteraction();

    RegressionModel *r = new RegressionModel;

    r->setName(mNameEdit->text());

    r->setInterceptTerm(mInterceptTerm->isChecked());

    QList<RegressionListItem*> dependent = checkedDependent();
    if(dependent.count() != 0)
    {
	r->addDependentVariables(dependent);
    }
    else
    {
	for(int i=0; i<mDependentSpectrogramList->count(); i++)
	{
	    if( mDependentSpectrogramList->item(i)->checkState() == Qt::Checked )
	    {
        r->setDependentSpectrogram(dynamic_cast<RegressionSpectrogramListItem*>(mDependentSpectrogramList->item(i))->data());
		break;
	    }
	}
    }
    r->addIndependentVariables(simple,interaction);

    emit regressionObject(r);

    close();
}

void RegressionDialog::setFromRegression(RegressionModel *model)
{
    mNameEdit->setText(model->name());
    mInterceptTerm->setChecked(model->hasIntercept());

    for(int i=0; i < mDependentList->count(); i++)
    if( model->mDependent.contains( dynamic_cast<RegressionListItem*>(mDependentList->item(i))->data() ) )
	    mDependentList->item(i)->setCheckState(Qt::Checked);

    if( model->dependentIsSpectrogram() )
	for(int i=0; i < mDependentSpectrogramList->count(); i++)
        if( model->mDependentSpectrogram == dynamic_cast<RegressionSpectrogramListItem*>(mDependentSpectrogramList->item(i))->data() )
		mDependentSpectrogramList->item(i)->setCheckState(Qt::Checked);

    if( model->mDependent.count() > 1 )
	mDependentSpectrogramList->setEnabled(false);
    if( model->dependentIsSpectrogram() )
	mDependentList->setEnabled(false);

    for(int i=0; i< mIndependentSimpleList->count(); i++ )
    if( model->mSimple.contains( dynamic_cast<RegressionListItem*>(mIndependentSimpleList->item(i))->data() ))
	    mIndependentSimpleList->item(i)->setCheckState(Qt::Checked);

    for(int i=0; i< model->mInteraction.count(); i++)
    {
	RegressionInteractionListItem *tmp = new RegressionInteractionListItem( model->mInteraction.at(i) );
	tmp->setCheckState(Qt::Checked);
	mIndependentInteractionList->addItem( tmp );
    }
}
