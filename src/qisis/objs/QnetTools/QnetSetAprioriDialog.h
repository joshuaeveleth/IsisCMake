#ifndef QnetSetAprioriDialog_h
#define QnetSetAprioriDialog_h

#include <QDialog>


//forward declarations
class QDialog;
class QGridLayout;
class QGroupBox;
class QLabel;
class QLineEdit;
class QListWidget;
class QListWidgetItem;
class QPushButton;
class QStackedWidget;
class QString;
class QStringList;
class QVBoxLayout;


namespace Isis {
  class QnetTool;

  /**
   * @author ????-??-?? Unknown
   *
   * @internal
   *   @history 2016-08-09 Makayla Shepherd - Complete redesign of the user interface. 
   *                           Fixes #2325, #2383.
   */
  class QnetSetAprioriDialog : public QDialog {
      Q_OBJECT

    public:
      QnetSetAprioriDialog(QnetTool *qnetTool, QWidget *parent = 0);
      void setPoints(QList<QListWidgetItem *> selectedPoints);
    
    public slots:
      void setVisiblity();
      
    signals:
      void pointChanged(QString pointId);
      void netChanged();

    private slots:
      void fillCurrentAprioriLineEdits();
      void fillReferenceAprioriLineEdits();
      void fillAverageAprioriLineEdits();
//      void fillGroundSourceAprioriLineEdits();
      void fillSigmaLineEdits();
      void clearLineEdits();
      void resetInfoLabels();
      void setApriori();
      
    private:
      
      void createSetAprioriDialog(QWidget *parent);
      void setInfoStack(QList<QListWidgetItem *> selectedPoints);
      void checkPointInfoDisable(QList<QListWidgetItem *> selectedPoints);
      
      QDialog *m_aprioriDialog;
      QGridLayout *m_aprioriGridLayout;
      QPushButton *m_okButton;
      QPushButton *m_cancelButton;
      QPushButton *m_applyButton;
      QStackedWidget *m_pointInfoStack;
      
      QGroupBox *m_singlePointInfoGroup;
      QLabel *m_pointIDLabel;
      QLabel *m_pointTypeLabel;
      QLabel *m_pointMeasureNumber;
      QLabel *m_editLockedBoolLabel;
      QLabel *m_ignoredBoolLabel;
      
      QGroupBox *m_multiplePointsInfoGroup;
      QLabel *m_pointsCount;
      QLabel *m_pointsMeasuresCount;
      QLabel *m_constrainedPointsCount;
      QLabel *m_fixedPointsCount;
      QLabel *m_freePointsCount;
      QLabel *m_pointsEditLockedCount;
      QLabel *m_pointsIgnoredCount;
      
      QGroupBox *m_pointGroup;
      QLabel *m_aprioriLatLabel;
      QLabel *m_aprioriLonLabel;
      QLabel *m_aprioriRadiusLabel;
      QLineEdit *m_latLineEdit;
      QLineEdit *m_lonLineEdit;
      QLineEdit *m_radiusLineEdit;
      QPushButton *m_currentAprioriButton;
      QPushButton *m_referenceAprioriButton;
      QPushButton *m_averageAprioriButton;
      
      QGroupBox *m_sigmaGroup;
      QLabel *m_sigmaWarningLabel;
      QPushButton *m_currentSigmaButton;
      QLabel *m_latSigmaLabel;
      QLabel *m_lonSigmaLabel;
      QLabel *m_radiusSigmaLabel;
      QLineEdit *m_latSigmaLineEdit;
      QLineEdit *m_lonSigmaLineEdit;
      QLineEdit *m_radiusSigmaLineEdit;
      
      QList<QListWidgetItem *> m_points;
      QnetTool *m_qnetTool;
      
      enum Source {
        USER,
        AVERAGE,
        REFERENCE,
      };
      
      Source m_aprioriSource;
      
      int m_multiPointsMeasureCount;
      int m_multiPointsConstraintedCount;
      int m_multiPointsFixedCount;
      int m_multiPointsFreeCount;
      int m_multiPointsEditLockedCount;
      int m_multiPointsIgnoredCount;
  };
}

#endif
