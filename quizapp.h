// QuizApp.h
#ifndef QUIZAPP_H
#define QUIZAPP_H

#include <QMainWindow>
#include <QList>
#include <QString>
#include <QStringList>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>
#include <QVector>
#include <QLineEdit>
#include <QLabel>

class Candidat {
public:
    QString nom;
    QString matricule;
    QString dateNaissance;
    double moyenne;
    bool aPasseExamen;
};

class Question {
public:
    QString texte;
    QStringList choix;
    int bonneReponse;
};

class QuizApp : public QMainWindow {
    Q_OBJECT

public:
    explicit QuizApp(QWidget *parent = nullptr);
    QLabel *backgroundLabel; // Pour afficher une image d'arrière-plan
    QLineEdit *lineEdit1;
    QLineEdit *lineEdit2;
    QPushButton *submitButton;

private slots:
    void afficherMenuPrincipal();
    void afficherMenuAdministrateur();
    void afficherMenuCandidat();
    void ajouterQuestion();
    void ajouterCandidat();
    void voirQuestions();
    void voirCandidats();
    void verifierInfos();
    void passerExamen(const QString &matricule);
    void voirResultat(const QString &matricule);
    void voirListeAttente();
    void voirListePrincipale();

private:
    QWidget *centralWidget;
    QVBoxLayout *mainLayout;
    QList<Candidat> candidats;
    QList<Question> questions;
    QList<QString> listeAttente;
    QList<QString> listePrincipale;
    int scoreActuel;

    Candidat* trouverCandidat(const QString &matricule);
    void setupUI();
    void creerMenuPrincipal();
    void creerMenuAdministrateur();
    void creerMenuCandidat();
    QVector<Question> melangerQuestions();
    void appliquerStyles();
    void configurerGeometrie();
};

#endif // QUIZAPP_H
