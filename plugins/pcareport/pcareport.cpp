#include <QtWidgets>
#include <QtDebug>
#include <QVariant>

#include <gsl/gsl_statistics_double.h>
#include <gsl/gsl_block.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_eigen.h>
#include <gsl/gsl_permutation.h>
#include <gsl/gsl_sort_vector.h>
#include <gsl/gsl_blas.h>

#include "pcareport.h"
#include <dataentrydialog.h>

#include <waveformdata.h>
#include <spectrogramdata.h>

PcaReportPlugin::PcaReportPlugin()
{
    settingsLabels << "How many components?";
    settingsValues << 5;

    pluginnames << "PCA Report & Data";
}

QString PcaReportPlugin::name() const
{
    return "PCA Plugin";
}

QString PcaReportPlugin::scriptName() const
{
    return "pcaLibrary";
}

QStringList PcaReportPlugin::names() const
{
    return pluginnames;
}

PcaReportPlugin* PcaReportPlugin::copy() const
{
    // this needs to be better
    return new PcaReportPlugin();
}

void PcaReportPlugin::settings(int i)
{
    DataEntryDialog dew(&settingsLabels, &settingsValues, "", 0);
    if( dew.exec() == QDialog::Accepted)
    {
	for(int i=0; i<settingsValues.count(); i++)
	{
	    settingsValues.replace(i, dew.values()->at(i));
	}
    }
}

void PcaReportPlugin::calculate(QString name, SpectrogramData *data)
{
    int index = pluginnames.indexOf(name);
    if(index != -1)
	calculate(index, data);
}

void PcaReportPlugin::calculate(int index, SpectrogramData *data)
{
    quint32 nrow = data->getNTimeSteps();
    quint32 ncol = data->getNFrequencyBins();
    double *matrix = (double*)malloc( sizeof(double) * nrow * ncol );
    gsl_matrix *cov = gsl_matrix_alloc(ncol,ncol);

    // means-subtracted
    // doesn't matter for the covariance, but is necessary for calculating the loadings later on
    for(quint32 j=0; j<ncol; j++) // for each column
    {
	double tmp = 0;
	for(quint32 i=0; i<nrow; i++) // for each row
	{
	    tmp += *(data->pdata()+i*ncol+j);
	}
	tmp /= nrow;
	for(quint32 i=0; i<nrow; i++) // for each row
	{
	    *(matrix+i*ncol+j) =  *(data->pdata()+i*ncol+j) - tmp;
	}
    }

    // a bit more efficient since the matrix is symmetrical
    for(quint32 i=0; i<ncol; i++)
    {
	for(quint32 j=0; j<=i; j++)
	{
	    // row i, column j
	    double tmp = gsl_stats_covariance (matrix+i, ncol, matrix+j, ncol, nrow);
	    gsl_matrix_set (cov, i, j, tmp);
	    // I think the eigen-whatever function will ignore half the matrix; perhaps one of these two calls can be taken out
	    gsl_matrix_set (cov, j, i, tmp);
	}
    }

    // eigen thing
    gsl_eigen_symmv_workspace *eigenWorkspace = gsl_eigen_symmv_alloc(ncol);
    gsl_matrix *eigenvectors = gsl_matrix_alloc(ncol,ncol);
    gsl_vector *eigenvalues = gsl_vector_alloc(ncol);
    gsl_eigen_symmv( cov , eigenvalues, eigenvectors, eigenWorkspace );

    // sort the eigenvectors and eigenvalues
    gsl_eigen_symmv_sort(eigenvalues, eigenvectors, GSL_EIGEN_SORT_VAL_DESC );

    gsl_matrix *transformed = gsl_matrix_alloc(ncol,nrow);
    gsl_matrix_view tmp = gsl_matrix_view_array(matrix, nrow, ncol);

    // multiply the matrices
    gsl_blas_dgemm (CblasTrans, CblasTrans, 1.0, eigenvectors, &tmp.matrix, 0.0, transformed);

    double *variances = (double*)malloc(sizeof(double)*ncol);
    double *cumulative_sum = (double*)malloc(sizeof(double)*ncol);
    double variance_sum = 0;
    for(quint32 i=0; i<ncol; i++)
    {
	// sort of a cheaty way to refer to a matrix row
	*(variances+i) = gsl_stats_variance( (transformed->data + nrow*i) , 1, nrow);
	variance_sum += *(variances+i);
    }
    *(variances+0) /= variance_sum;
    *(cumulative_sum+0) = *(variances+0);
    for(quint32 i=1; i<ncol; i++)
    {
	*(variances+i) /= variance_sum;
	*(cumulative_sum+i) = *(variances+i) + *(cumulative_sum+i-1);
    }

    // in case more components are requested than there are dimensions
    quint32 requested = settingsValues.at(0).toInt();
    if(requested > ncol)
	requested = ncol;

//    QList<WaveformData*> toReturn;
    for(quint32 i=0; i<requested; i++)
    {
	double *x = (double*)malloc(sizeof(double)*nrow);
	double *y = (double*)malloc(sizeof(double)*nrow);
	for(quint32 j=0; j<nrow; j++)
	{
	    *(x+j) = data->getTimeFromIndex(j);
	    *(y+j) = gsl_matrix_get(transformed,i,j);
	}
	emit waveformCreated( new WaveformData(tr("PC ")+QString::number(i+1),x,y,nrow,0) );
    }

    QDialog *dlg = new QDialog;
    QVBoxLayout *layout = new QVBoxLayout;
    QTextEdit *edit = new QTextEdit;
    layout->addWidget(edit);
    QString reportText("Parameter\t% Variance\tCum. Sum\n");
    for(quint32 j=0; j<ncol; j++)
	reportText += QString::number(j+1) + "\t" + QString::number(*(variances+j)) + "\t" + QString::number(*(cumulative_sum+j)) + "\n";
    edit->setText(reportText);
    dlg->setLayout(layout);
    dlg->setWindowTitle(tr("PCA Report"));
//    dlg->exec();
//    dlg->setModal(true);
    dlg->show();

    gsl_eigen_symmv_free(eigenWorkspace);
    gsl_matrix_free(eigenvectors);
    gsl_vector_free(eigenvalues);
    gsl_matrix_free(transformed);

    free(variances);
    free(cumulative_sum);
    free(matrix);
    free(cov);
}

void PcaReportPlugin::setParameter(QString label, QVariant value)
{
    int index = settingsLabels.indexOf(label);
    if(index != -1)
	settingsValues[index] = value;
}

