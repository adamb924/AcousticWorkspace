#include "regression.h"

#include <gsl/gsl_block.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_sf_bessel.h>
#include <gsl/gsl_multifit.h>
#include <gsl/gsl_statistics_double.h>

#include <qwt_plot.h>
#include <qwt_plot_curve.h>

#include <QtWidgets>
#include <QtDebug>
#include <QTime>
#include <QDir>
#include <QFileInfo>

InteractionEffect::InteractionEffect()
{

}


QString InteractionEffect::name()
{
    QString ret;
    for(int j=0; j < members.count(); j++)
	ret += members.at(j)->name() + ":";
    ret.replace(QRegExp(":$"),"");

    return ret;
}

QString InteractionEffect::safeName()
{
    QString ret;
    for(int j=0; j < members.count(); j++)
	ret += members.at(j)->safeName() + ":";
    ret.replace(QRegExp(":$"),"");

    return ret;
}

RegressionModel::RegressionModel() : mDependentSpectrogram(0), mInterceptTerm(true), mSpectrogramMode(false), mLabel("Default name")
{
}

RegressionModel::~RegressionModel()
{
}

bool RegressionModel::hasIntercept() const
{
    return mInterceptTerm;
}

void RegressionModel::setInterceptTerm(bool value)
{
    mInterceptTerm = value;
}

void RegressionModel::setDependentSpectrogram(SpectrogramData *dep)
{
    mDependentSpectrogram = dep;
    mSpectrogramMode = true;
}

void RegressionModel::addDependentVariables(QList<RegressionListItem*> dep)
{
    for(int i=0; i<dep.count(); i++)
	mDependent.append(dep.at(i)->data());
    mSpectrogramMode = false;
}

void RegressionModel::addIndependentVariables(QList<RegressionListItem*> simple, QList<RegressionInteractionListItem*> interaction)
{
    for(int i=0; i<simple.count(); i++)
	this->mSimple.append(simple.at(i)->data());

    // make sure that redundant items aren't added
    for(int i=0; i<interaction.count(); i++)
    {
	bool redundant = false;
	for(int j=0; j<this->mInteraction.count(); j++)
	{
	    if( this->mInteraction.at(j)->members == interaction.at(i)->interaction()->members )
	    {
		redundant = true;
		break;
	    }
	}
	if(!redundant)
	    this->mInteraction.append(interaction.at(i)->interaction());
    }
}

void RegressionModel::fit()
{
    if( (!mSpectrogramMode && mDependent.count() == 0) || (mSimple.count() + mInteraction.count())==0 ) { return; }

    int nrow;
    if(mSpectrogramMode)
	nrow = mDependentSpectrogram->getNTimeSteps();
    else
	nrow = mDependent.at(0)->getNSamples();

    int ncol = mSimple.count() + mInteraction.count();
    if(mInterceptTerm)
	ncol++;

    gsl_multifit_linear_workspace *workspace = gsl_multifit_linear_alloc(nrow,ncol);
    gsl_vector *residuals = gsl_vector_alloc(nrow);
    gsl_vector *estimate = gsl_vector_alloc(ncol);
    gsl_matrix *cov = gsl_matrix_alloc(ncol,ncol);
    double chisq;

    QStringList factorNames;

    gsl_matrix *independent = gsl_matrix_alloc(nrow,ncol);
    int ct=0;
    for(int i=0; i<mSimple.count(); i++)
    {
	factorNames << mSimple.at(i)->name();

    gsl_vector_const_view tmpView = gsl_vector_const_view_array( mSimple.at(i)->yData().data() , nrow);
	gsl_matrix_set_col(independent, ct, &tmpView.vector );
	ct++;
    }

    for(int i=0; i<mInteraction.count(); i++)
    {
	// skip an empty interaction
	if( mInteraction.at(i)->members.count() == 0) { continue; }

	factorNames << mInteraction.at(i)->name();

	gsl_vector *tmp = gsl_vector_alloc(nrow);

    gsl_vector_const_view firstView = gsl_vector_const_view_array( mInteraction.at(i)->members.at(0)->yData().data() , nrow);

	gsl_vector_memcpy(tmp, &firstView.vector);

	for(int j=1; j<mInteraction.at(i)->members.count(); j++)
	{
        gsl_vector_const_view tmpView = gsl_vector_const_view_array( mInteraction.at(i)->members.at(j)->yData().data() , nrow);
	    gsl_vector_mul ( tmp , &(tmpView.vector) );
	}

	gsl_matrix_set_col(independent, ct, tmp );

	gsl_vector_free(tmp);

	ct++;
    }

    if(mInterceptTerm)
    {
	for(int i=0; i<nrow; i++)
	    gsl_matrix_set(independent,i,ct,1);
	factorNames << "Intercept";
    }

    QString output = "";


    if(mSpectrogramMode)
    {
	double *rsq = (double*)malloc(sizeof(double)*mDependentSpectrogram->getNFrequencyBins());

	output += "Freq.Bin\tR-squared\tRSS\tTSS\n";
	for(quint32 i=0; i< mDependentSpectrogram->getNFrequencyBins(); i++ )
	{
	    gsl_vector_view dependent_view = gsl_vector_view_array_with_stride( mDependentSpectrogram->pdata()+i, mDependentSpectrogram->getNFrequencyBins(), mDependentSpectrogram->getNTimeSteps() );

	    gsl_multifit_linear(independent, &dependent_view.vector, estimate, cov, &chisq, workspace);
	    gsl_multifit_linear_residuals(independent, &dependent_view.vector, estimate, residuals);

	    double TSS = gsl_stats_tss(mDependentSpectrogram->pdata()+i,mDependentSpectrogram->getNFrequencyBins(),mDependentSpectrogram->getNTimeSteps());
	    double RSS = gsl_stats_tss(residuals->data,1,nrow);
	    *(rsq+i) = 1- RSS/TSS;

	    output += QString::number(mDependentSpectrogram->getFrequencyFromIndex(i)) + "\t" + QString::number(*(rsq+i)) + "\t" + QString::number(RSS) + "\t" + QString::number(TSS) + "\n";
	}

	QwtPlot *qwtPlot = new QwtPlot;
	qwtPlot->enableAxis(QwtPlot::yLeft);
	QwtPlotCurve *waveCurve = new QwtPlotCurve("dummy");
	waveCurve->setRenderHint(QwtPlotItem::RenderAntialiased);
	waveCurve->setPen(QPen(Qt::blue));
    waveCurve->setSamples(mDependentSpectrogram->pfrequencies(),rsq,mDependentSpectrogram->getNFrequencyBins());
	waveCurve->attach(qwtPlot);

	qwtPlot->setWindowTitle("R-squared values");
	qwtPlot->setWindowFlags(Qt::Window);
	qwtPlot->show();


	free(rsq);
    }
    else
    {
	for(int i=0; i< mDependent.count(); i++ )
	{
	    output += "Report for: "+ mDependent.at(i)->name() +"\n";

        gsl_vector_const_view dependent_view = gsl_vector_const_view_array(mDependent.at(i)->yData().data(), nrow);

	    gsl_multifit_linear(independent, &dependent_view.vector, estimate, cov, &chisq, workspace);
	    gsl_multifit_linear_residuals(independent, &dependent_view.vector, estimate, residuals);
        double TSS = gsl_stats_tss(mDependent.at(i)->yData().data(),1,nrow);
	    double rsq = 1- gsl_stats_tss(residuals->data,1,nrow)/TSS;

	    output += "Estimates\n";
	    for(size_t j=0; j < estimate->size; j++)
		output +=  factorNames.at(j) + "\t" + QString::number(gsl_vector_get(estimate,j)) + "\n";
	    output += "R-squared:\t" + QString::number(rsq) + "\n";
	    output += "\n";
	}
    }

    QTextEdit *edit = new QTextEdit;
    edit->setText(output);
    edit->setWindowTitle("Results summary");
    edit->setWindowFlags(Qt::Window);
    edit->show();

    gsl_matrix_free(cov);
    gsl_matrix_free(independent);
    gsl_vector_free(estimate);
    gsl_vector_free(residuals);
    gsl_multifit_linear_free(workspace);
}

void RegressionModel::R()
{
    QString originalWorkingDirectory = QDir::current().absolutePath();
    QDir dir = QDir::current();
    QString dirName = QTime::currentTime().toString("t_hhmmsszzz");;
    dir.mkdir(dirName);
    dir.cd(dirName);
    QDir::setCurrent(dir.absolutePath());

    QString code;

    QString equation = "";

    // write dependent variable
    if(mSpectrogramMode)
    {
	QFile file(mDependentSpectrogram->safeName());
	QFileInfo info(file);

	code += mDependentSpectrogram->safeName() + " <- readBin(con='"+info.absoluteFilePath()+"', what=double(), n = "+QString::number(mDependentSpectrogram->getNTimeSteps()*mDependentSpectrogram->getNFrequencyBins())+", size = 4);\n";
	code += mDependentSpectrogram->safeName() + " <- matrix(data = "+mDependentSpectrogram->safeName()+", nrow = "+QString::number(mDependentSpectrogram->getNFrequencyBins())+", ncol = "+QString::number(mDependentSpectrogram->getNTimeSteps())+", byrow = F);\n";

	if( file.open(QIODevice::WriteOnly) == false ) { qCritical() << "Error opening " + mDependentSpectrogram->safeName(); return; }
	QDataStream out(&file);
	out.setByteOrder(QDataStream::LittleEndian);
	out.setFloatingPointPrecision(QDataStream::SinglePrecision);
	for(quint32 j=0; j< mDependentSpectrogram->getNTimeSteps()*mDependentSpectrogram->getNFrequencyBins(); j++)
	{
	    out << *(mDependentSpectrogram->pdata()+j);
	}

	file.close();
    }
    else
    {
	for(int i=0; i<mDependent.count(); i++)
	{
	    QFile file(mDependent.at(i)->safeName());
	    QFileInfo info(file);

	    code += mDependent.at(i)->safeName() + " <- readBin(con='"+info.absoluteFilePath()+"', what=double(), n = "+QString::number(mDependent.at(i)->getNSamples())+", size = 4);\n";

	    if( file.open(QIODevice::WriteOnly) == false ) { qCritical() << "Error opening " + mDependent.at(i)->safeName(); return; }
	    QDataStream out(&file);
	    out.setByteOrder(QDataStream::LittleEndian);
	    out.setFloatingPointPrecision(QDataStream::SinglePrecision);
	    for(quint32 j=0; j< mDependent.at(i)->getNSamples(); j++)
	    {
        out << mDependent.at(i)->yData().at(j);
	    }

	    file.close();
	}
    }

    // write independent variables
    for(int i=0; i<mSimple.count(); i++)
    {
	QFile file(mSimple.at(i)->safeName());
	QFileInfo info(file);

	code += mSimple.at(i)->safeName() + " <- readBin(con='"+info.absoluteFilePath()+"', what=double(), n = "+QString::number(mSimple.at(i)->getNSamples())+", size = 4);\n";

	if( file.open(QIODevice::WriteOnly) == false ) { qCritical() << "Error opening " + mSimple.at(i)->safeName(); return; }
	QDataStream out(&file);
	out.setByteOrder(QDataStream::LittleEndian);
	out.setFloatingPointPrecision(QDataStream::SinglePrecision);
	for(quint32 j=0; j< mSimple.at(i)->getNSamples(); j++)
	{
        out << mSimple.at(i)->yData().at(j);
	}

	file.close();

	equation += mSimple.at(i)->safeName() + " + ";
    }

    for(int i=0; i<mInteraction.count(); i++)
    {
	for(int j=0; j < mInteraction.at(i)->members.count(); j++)
	    equation += mInteraction.at(i)->members.at(j)->safeName() + ":";
	equation.replace(QRegExp(":$"),"");

	equation += " + ";
    }

    if( mInterceptTerm )
	equation += "1";
    else
	equation += "0";

    if(mSpectrogramMode)
    {
	code += "models <- list();\nsummaries <- list();\n";
	for(quint32 i=1; i< mDependentSpectrogram->getNFrequencyBins(); i++)
	{
	    code += "models[["+QString::number(i)+"]] <- lm( "+mDependentSpectrogram->safeName()+"["+QString::number(i)+",] ~ "+equation+" );\n";
	    code += "summaries[["+QString::number(i)+"]] <- summary(lm( "+mDependentSpectrogram->safeName()+"["+QString::number(i)+",] ~ "+equation+" ));\n";
	}
    }
    else
    {
	for(int i=0; i<mDependent.count(); i++)
	    code += "model"+QString::number(i+1)+" <- lm("+ mDependent.at(i)->safeName() +" ~ "+equation+");\nsummary(model"+QString::number(i+1)+");\n";
    }

    QFile file("code.RCode");
    if( file.open(QIODevice::WriteOnly) == false ) { qCritical() << "Error opening code.RCode"; return; }
    QTextStream out(&file);
    out << code;
    file.close();

    QTextEdit *edit = new QTextEdit;
    edit->setText(code);
    edit->setWindowTitle("R Code");
    edit->setWindowFlags(Qt::Window);
    edit->show();

    QDir::setCurrent(originalWorkingDirectory);
}

void RegressionModel::setName(QString n)
{
    mLabel = n;
}


QString RegressionModel::name() const
{
    return mLabel;
}

bool RegressionModel::dependentIsSpectrogram() const
{
    return mSpectrogramMode;
}
