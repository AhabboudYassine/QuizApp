// QuizApp.cpp
#include "QuizApp.h"
#include <QInputDialog>
#include <QMessageBox>
#include <QRandomGenerator>
#include <QTextStream>
#include <QPushButton>
#include <QLabel>
#include <algorithm>
#include <QVBoxLayout>

QuizApp::QuizApp(QWidget *parent) : QMainWindow(parent), scoreActuel(0) {
    setWindowTitle("Quiz Application");
    setupUI();
    afficherMenuPrincipal();
}

void QuizApp::setupUI() {

    resize(1024, 768);

    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    mainLayout = new QVBoxLayout(centralWidget);

    // Ajouter les marges et l'espacement
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(15);

    // Appliquer les styles et la géométrie
    appliquerStyles();
    configurerGeometrie();

}

void QuizApp::creerMenuPrincipal() {
    while (QLayoutItem* item = mainLayout->takeAt(0)) {
        delete item->widget();
        delete item;
    }

    QLabel *titre = new QLabel("Menu Principal", centralWidget);
    titre->setAlignment(Qt::AlignCenter);
    titre->setMinimumHeight(60);
    mainLayout->addWidget(titre);

    QPushButton *btnAdmin = new QPushButton("Mode Administrateur", centralWidget);
    QPushButton *btnCandidat = new QPushButton("Mode Candidat", centralWidget);
    QPushButton *btnQuitter = new QPushButton("Quitter", centralWidget);

    QList<QPushButton*> buttons = {btnAdmin, btnCandidat, btnQuitter};
    for(QPushButton* btn : buttons) {
        btn->setMinimumWidth(300);
        btn->setMinimumHeight(50);
        mainLayout->addWidget(btn);
    }

    mainLayout->addStretch();

    connect(btnAdmin, &QPushButton::clicked, this, &QuizApp::afficherMenuAdministrateur);
    connect(btnCandidat, &QPushButton::clicked, this, &QuizApp::afficherMenuCandidat);
    connect(btnQuitter, &QPushButton::clicked, this, &QMainWindow::close);
}

void QuizApp::afficherMenuPrincipal() {
    creerMenuPrincipal();
}

void QuizApp::afficherMenuAdministrateur() {
    bool ok;
    QString password = QInputDialog::getText(this, "Mode Administrateur",
                                             "Entrez le mot de passe :", QLineEdit::Password, "", &ok);

    if (!ok || password != "0000") {
        QMessageBox::warning(this, "Erreur", "Mot de passe incorrect !");
        return;
    }

    while (QLayoutItem* item = mainLayout->takeAt(0)) {
        delete item->widget();
        delete item;
    }

    QLabel *titre = new QLabel("Menu Administrateur", centralWidget);
    titre->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titre);

    QPushButton *btnAjoutQuestion = new QPushButton("Ajouter une question", centralWidget);
    QPushButton *btnAjoutCandidat = new QPushButton("Ajouter un candidat", centralWidget);
    QPushButton *btnVoirQuestions = new QPushButton("Voir les questions", centralWidget);
    QPushButton *btnVoirCandidats = new QPushButton("Voir les candidats", centralWidget);
    QPushButton *btnListeAttente = new QPushButton("Voir liste d'attente", centralWidget);
    QPushButton *btnListePrincipale = new QPushButton("Voir liste principale", centralWidget);
    QPushButton *btnRetour = new QPushButton("Retour", centralWidget);

    mainLayout->addWidget(btnAjoutQuestion);
    mainLayout->addWidget(btnAjoutCandidat);
    mainLayout->addWidget(btnVoirQuestions);
    mainLayout->addWidget(btnVoirCandidats);
    mainLayout->addWidget(btnListeAttente);
    mainLayout->addWidget(btnListePrincipale);
    mainLayout->addWidget(btnRetour);

    connect(btnAjoutQuestion, &QPushButton::clicked, this, &QuizApp::ajouterQuestion);
    connect(btnAjoutCandidat, &QPushButton::clicked, this, &QuizApp::ajouterCandidat);
    connect(btnVoirQuestions, &QPushButton::clicked, this, &QuizApp::voirQuestions);
    connect(btnVoirCandidats, &QPushButton::clicked, this, &QuizApp::voirCandidats);
    connect(btnListeAttente, &QPushButton::clicked, this, &QuizApp::voirListeAttente);
    connect(btnListePrincipale, &QPushButton::clicked, this, &QuizApp::voirListePrincipale);
    connect(btnRetour, &QPushButton::clicked, this, &QuizApp::afficherMenuPrincipal);
}

void QuizApp::afficherMenuCandidat() {
    while (QLayoutItem* item = mainLayout->takeAt(0)) {
        delete item->widget();
        delete item;
    }

    QLabel *titre = new QLabel("Menu Candidat", centralWidget);
    titre->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titre);

    QPushButton *btnVerifInfo = new QPushButton("Vérifier mes informations", centralWidget);
    QPushButton *btnPasserExam = new QPushButton("Passer l'examen", centralWidget);
    QPushButton *btnVoirResult = new QPushButton("Voir mon résultat", centralWidget);
    QPushButton *btnRetour = new QPushButton("Retour", centralWidget);

    mainLayout->addWidget(btnVerifInfo);
    mainLayout->addWidget(btnPasserExam);
    mainLayout->addWidget(btnVoirResult);
    mainLayout->addWidget(btnRetour);

    connect(btnVerifInfo, &QPushButton::clicked, this, &QuizApp::verifierInfos);
    connect(btnPasserExam, &QPushButton::clicked, [this]() {
        bool ok;
        QString matricule = QInputDialog::getText(this, "Identification",
                                                  "Entrez votre matricule :", QLineEdit::Normal, "", &ok);
        if (ok && !matricule.isEmpty()) {
            passerExamen(matricule);
        }
    });
    connect(btnVoirResult, &QPushButton::clicked, [this]() {
        bool ok;
        QString matricule = QInputDialog::getText(this, "Identification",
                                                  "Entrez votre matricule :", QLineEdit::Normal, "", &ok);
        if (ok && !matricule.isEmpty()) {
            voirResultat(matricule);
        }
    });
    connect(btnRetour, &QPushButton::clicked, this, &QuizApp::afficherMenuPrincipal);
}

void QuizApp::ajouterQuestion() {
    bool ok;
    QString texte = QInputDialog::getText(this, "Ajouter Question", "Texte de la question :", QLineEdit::Normal, "", &ok);
    if (!ok || texte.isEmpty()) return;

    QStringList choix;
    for (int i = 1; i <= 4; ++i) {
        QString choixTexte = QInputDialog::getText(this, "Ajouter Question", QString("Choix %1 :").arg(i), QLineEdit::Normal, "", &ok);
        if (!ok || choixTexte.isEmpty()) return;
        choix.append(choixTexte);
    }

    int bonneReponse = QInputDialog::getInt(this, "Ajouter Question", "Index de la bonne réponse (1-4) :", 1, 1, 4, 1, &ok);
    if (!ok) return;

    Question question;
    question.texte = texte;
    question.choix = choix;
    question.bonneReponse = bonneReponse - 1;
    questions.append(question);
    QMessageBox::information(this, "Succès", "Question ajoutée avec succès !");
}

void QuizApp::ajouterCandidat() {
    bool ok;
    QString nom = QInputDialog::getText(this, "Ajouter Candidat", "Nom du candidat :", QLineEdit::Normal, "", &ok);
    if (!ok || nom.isEmpty()) return;

    QString matricule = QInputDialog::getText(this, "Ajouter Candidat", "Matricule :", QLineEdit::Normal, "", &ok);
    if (!ok || matricule.isEmpty()) return;

    QString dateNaissance = QInputDialog::getText(this, "Ajouter Candidat", "Date de naissance (JJ/MM/AAAA) :", QLineEdit::Normal, "", &ok);
    if (!ok || dateNaissance.isEmpty()) return;

    Candidat candidat;
    candidat.nom = nom;
    candidat.matricule = matricule;
    candidat.dateNaissance = dateNaissance;
    candidat.moyenne = 0.0;
    candidat.aPasseExamen = false;

    candidats.append(candidat);
    QMessageBox::information(this, "Succès", "Candidat ajouté avec succès !");
}

QVector<Question> QuizApp::melangerQuestions() {
    QVector<Question> questionsAleatoires;

    // Copier toutes les questions dans le vecteur
    for (const Question& q : questions) {
        questionsAleatoires.append(q);
    }

    // Mélanger le vecteur des questions
    for (int i = questionsAleatoires.size() - 1; i > 0; --i) {
        int j = QRandomGenerator::global()->bounded(i + 1);
        std::swap(questionsAleatoires[i], questionsAleatoires[j]);

        // Mélanger les choix de réponse pour chaque question
        QStringList& choix = questionsAleatoires[i].choix;
        int bonneReponseOriginal = questionsAleatoires[i].bonneReponse;
        QString bonneReponseTexte = choix[bonneReponseOriginal];

        // Mélanger les choix
        for (int k = choix.size() - 1; k > 0; --k) {
            int l = QRandomGenerator::global()->bounded(k + 1);
            choix.swapItemsAt(k, l);
        }

        // Mettre à jour l'index de la bonne réponse
        questionsAleatoires[i].bonneReponse = choix.indexOf(bonneReponseTexte);
    }

    return questionsAleatoires;
}

void QuizApp::voirQuestions() {
    QString texte = "Liste des questions :\n\n";
    for (const auto &q : questions) {
        texte += QString("Question : %1\nChoix : %2\nBonne réponse : Choix %3\n\n")
                     .arg(q.texte)
                     .arg(q.choix.join(", "))
                     .arg(q.bonneReponse + 1);
    }
    QMessageBox::information(this, "Questions", texte);
}

void QuizApp::voirCandidats() {
    QString texte = "Liste des candidats :\n\n";
    for (const auto &c : candidats) {
        texte += QString("Nom : %1\nMatricule : %2\n\n").arg(c.nom).arg(c.matricule);
    }
    QMessageBox::information(this, "Candidats", texte);
}

void QuizApp::verifierInfos() {
    bool ok;
    QString matricule = QInputDialog::getText(this, "Identification", "Entrez votre matricule :", QLineEdit::Normal, "", &ok);
    if (!ok || matricule.isEmpty()) return;

    Candidat *c = trouverCandidat(matricule);
    if (!c) {
        QMessageBox::warning(this, "Erreur", "Candidat introuvable !");
        return;
    }

    QString info = QString("Nom : %1\nMatricule : %2\nDate de naissance : %3")
                       .arg(c->nom)
                       .arg(c->matricule)
                       .arg(c->dateNaissance);
    QMessageBox::information(this, "Informations Candidat", info);
}

void QuizApp::passerExamen(const QString &matricule) {
    Candidat *c = trouverCandidat(matricule);
    if (!c) {
        QMessageBox::warning(this, "Erreur", "Candidat introuvable !");
        return;
    }
    if (c->aPasseExamen) {
        QMessageBox::warning(this, "Erreur", "Vous avez déjà passé l'examen !");
        return;
    }

    int score = 0;
    int totalQuestions = questions.size();

    if (totalQuestions == 0) {
        QMessageBox::warning(this, "Erreur", "Aucune question n'est disponible !");
        return;
    }

    // Obtenir les questions mélangées
    QVector<Question> questionsAleatoires = melangerQuestions();

    for (int i = 0; i < questionsAleatoires.size(); ++i) {
        const Question &q = questionsAleatoires[i];

        // Afficher la question et les choix
        QString questionText = QString("Question %1/%2:\n%3\n\nChoix disponibles:\n")
                                   .arg(i + 1)
                                   .arg(totalQuestions)
                                   .arg(q.texte);

        for (int j = 0; j < q.choix.size(); ++j) {
            questionText += QString("%1. %2\n").arg(j + 1).arg(q.choix[j]);
        }

        bool ok;
        int reponseIndex = QInputDialog::getInt(this, "Question",
                                                questionText,
                                                1, 1, q.choix.size(), 1, &ok) - 1;

        if (!ok) return;  // L'utilisateur a annulé

        if (reponseIndex == q.bonneReponse) {
            score++;
        }
    }

    // Calcul du pourcentage
    double pourcentage = (static_cast<double>(score) / totalQuestions) * 100;
    c->moyenne = pourcentage;
    c->aPasseExamen = true;

    QString messageResultat = QString("Résultat:\nQuestions correctes: %1/%2\nPourcentage: %3%")
                                  .arg(score)
                                  .arg(totalQuestions)
                                  .arg(QString::number(pourcentage, 'f', 1));

    QMessageBox::information(this, "Résultat de l'examen", messageResultat);

    if (pourcentage < 50) {
        listeAttente.append(c->nom);
    } else {
        listePrincipale.append(c->nom);
    }
}

void QuizApp::voirResultat(const QString &matricule) {
    Candidat *c = trouverCandidat(matricule);
    if (!c) {
        QMessageBox::warning(this, "Erreur", "Candidat introuvable !");
        return;
    }
    if (!c->aPasseExamen) {
        QMessageBox::warning(this, "Erreur", "Vous n'avez pas encore passé l'examen !");
        return;
    }

    QString resultat = QString("Nom : %1\nMatricule : %2\nRésultat : %3%\nStatut : %4")
                           .arg(c->nom)
                           .arg(c->matricule)
                           .arg(QString::number(c->moyenne, 'f', 1))
                           .arg(c->moyenne >= 50 ? "Admis" : "Non admis");
    QMessageBox::information(this, "Résultat", resultat);
}

void QuizApp::voirListeAttente() {
    QString texte = "Candidats en liste d'attente (< 50%) :\n\n";
    if (listeAttente.isEmpty()) {
        texte += "Aucun candidat en liste d'attente";
    } else {
        for (const QString &nom : listeAttente) {
            texte += "- " + nom + "\n";
        }
    }
    QMessageBox::information(this, "Liste d'attente", texte);
}

void QuizApp::voirListePrincipale() {
    QString texte = "Candidats admis (≥ 50%) :\n\n";
    if (listePrincipale.isEmpty()) {
        texte += "Aucun candidat admis";
    } else {
        for (const QString &nom : listePrincipale) {
            texte += "- " + nom + "\n";
        }
    }
    QMessageBox::information(this, "Liste Principale", texte);
}

Candidat* QuizApp::trouverCandidat(const QString &matricule) {
    for (Candidat &c : candidats) {
        if (c.matricule == matricule) {
            return &c;
        }
    }
    return nullptr;
}
void QuizApp::appliquerStyles() {
    centralWidget->setStyleSheet(
        "QWidget {"
        "    background-image: url(:/image/tss.png);"
        "    background-position: center;"
        "    background-repeat: no-repeat;"
        "    background-size: cover;"  // Permet de couvrir toute la zone
        "}"
        "QLabel {"
        "    color: white;"                  // Couleur du texte des labels en blanc
        "    font-family: 'Times New Roman';" // Police Times New Roman
        "    font-size: 50px;"               // Taille du texte
        "    font-weight: bold;"             // Texte en gras
        "    font-style: italic;"            // Texte en italique
        "    padding: 10px;"                 // Espacement autour du texte
        "    background: transparent;"      // Pas de fond pour le QLabel
        "}"
        "QPushButton {"
        "    background-color: blue;"   // Fond blanc
        "    color: white;"              // Texte en blanc
        "    font-weight: bold;"         // Texte en gras
        "    border: 2px solid #bdc3c7;" // Bordure grise claire
        "    border-radius: 20px;"        // Coins arrondis
        "    padding: 10px 20px;"        // Espacement intérieur
        "    font-size: 25px;"
        "    min-width: 200px;"          // Largeur minimale
        "    min-height: 20px;"          // Hauteur minimale
        "    margin: 5px;"
        "}"
        "QPushButton:hover {"
        "    background-color: #ecf0f1;" // Fond légèrement gris au survol
        "    color: #2c3e50;"           // Texte devient gris foncé au survol
        "}"
        "QPushButton:pressed {"
        "    background-color: #dfe6e9;" // Fond gris clair lorsqu'on appuie
        "    color: #2c3e50;"           // Texte reste gris foncé
        "}"
        "QMessageBox {"
        "    background-color: white;"
        "    color: white;"              // Texte en blanc
        "    font-weight: bold;"         // Texte en gras
        "}"
        "QInputDialog {"
        "    background-color: white;"
        "    color: white;"              // Texte en blanc
        "    font-weight: bold;"         // Texte en gras
        "}"
        );

}

void QuizApp::configurerGeometrie() {
    setStyleSheet(
        "QPushButton {"
        "    margin: 20px;"
        "    padding: 15px 30px;"
        "}"
        );
}
