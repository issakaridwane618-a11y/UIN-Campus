#include <algorithm>
#include <array>
#include <cctype>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <limits>
#include <locale>
#include <sstream>
#include <string>
#include <vector>
// TP 3 - Statistiques d'une serie de notes (C++23)
enum class Mention {
    Ajourne,
    Passable,
    AssezBien,
    Bien,
    TresBien
};
constexpr std::size_t nbMentions = 5;
constexpr double noteMin = 0.0;
constexpr double noteMax = 20.0;
constexpr double seuilValidation = 10.0;
// Classe une note dans l'une des cinq mentions.
Mention mentionPour(double note)
{
    if (note < seuilValidation) return Mention::Ajourne;
    if (note < 12.0) return Mention::Passable;
    if (note < 14.0) return Mention::AssezBien;
    if (note < 16.0) return Mention::Bien;
    return Mention::TresBien;
}
std::string libelle(Mention mention)
{
    switch (mention) {
        case Mention::Ajourne:   return "Ajourne";
        case Mention::Passable:  return "Passable";
        case Mention::AssezBien: return "Assez bien";
        case Mention::Bien:      return "Bien";
        case Mention::TresBien:  return "Tres bien";
    }
    return "Inconnue";
}
// Lit un entier compris entre min et max.
int lireEntier(const std::string& invite, int min, int max)
{
    while (true) {
        std::cout << invite;
        std::string ligne;
        if (!std::getline(std::cin, ligne)) {
            std::cerr << "\nFin de saisie inattendue.\n";
            std::exit(EXIT_FAILURE);
        }
        std::istringstream flux(ligne);
        int valeur{};
        char reste{};
        if ((flux >> valeur) && !(flux >> reste) &&
            valeur >= min && valeur <= max) {
            return valeur;
        }
        std::cout << "Saisie invalide. Entrez un entier entre "
                  << min << " et " << max << ".\n";
    }
}
// Lit une note entre 0 et 20. La virgule decimale est acceptee.
double lireNote(const std::string& invite)
{
    while (true) {
        std::cout << invite;
        std::string ligne;
        if (!std::getline(std::cin, ligne)) {
            std::cerr << "\nFin de saisie inattendue.\n";
            std::exit(EXIT_FAILURE);
        }
        for (char& c : ligne) {
            if (c == ',') c = '.';
        }
        std::istringstream flux(ligne);
        double note{};
        char reste{};
        if ((flux >> note) && !(flux >> reste) &&
            std::isfinite(note) && note >= noteMin && note <= noteMax) {
            return note;
        }
        std::cout << "Saisie invalide. La note doit etre comprise entre "
                  << noteMin << " et " << noteMax << ".\n";
    }
}
int main()
{
    std::cout << "TP 3 - Statistiques d'une serie de notes\n";
    std::cout << "----------------------------------------\n";
    std::cout << "Nom de l'UE : ";
    std::string ue;
    if (!std::getline(std::cin, ue)) {
        std::cerr << "Lecture du nom de l'UE impossible.\n";
        return EXIT_FAILURE;
    }
    const int nb = lireEntier(
        "Nombre d'etudiants (1 a 200) : ", 1, 200
    );
    std::vector<double> notes;
    notes.reserve(static_cast<std::size_t>(nb));
    for (int i = 1; i <= nb; ++i) {
        std::ostringstream invite;
        invite << "Note de l'etudiant " << i << " (0 a 20) : ";
        notes.push_back(lireNote(invite.str()));
    }
    double minimum = notes.front();
    double maximum = notes.front();
    double somme = 0.0;
    int admis = 0;
    std::array<int, nbMentions> repartition{};
    for (double note : notes) {
        minimum = std::min(minimum, note);
        maximum = std::max(maximum, note);
        somme += note;
        if (note >= seuilValidation) {
            ++admis;
        }
        const auto indice =
            static_cast<std::size_t>(mentionPour(note));
        ++repartition[indice];
    }
    const double taille = static_cast<double>(notes.size());
    const double moyenne = somme / taille;
    // Ecart-type de la serie : racine de la moyenne des carres des ecarts.
    double sommeEcarts = 0.0;
    for (double note : notes) {
        const double ecart = note - moyenne;
        sommeEcarts += ecart * ecart;
    }
    const double ecartType = std::sqrt(sommeEcarts / taille);
    const double pourcentageAdmis = 100.0 * admis / taille;
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "\nStatistiques - " << ue << " (" << nb
              << " etudiants)\n";
    std::cout << "----------------------------------------\n";
    std::cout << std::left << std::setw(20) << "Note minimale"
              << ": " << minimum << '\n';
    std::cout << std::left << std::setw(20) << "Note maximale"
              << ": " << maximum << '\n';
    std::cout << std::left << std::setw(20) << "Moyenne"
              << ": " << moyenne << '\n';
    std::cout << std::left << std::setw(20) << "Ecart-type"
              << ": " << ecartType << '\n';
    std::cout << std::left << std::setw(20) << "Admis"
              << ": " << admis << " (" << pourcentageAdmis << " %)\n";
    std::cout << "\nRepartition par mention\n";
    std::cout << "----------------------------------------\n";
    for (std::size_t i = 0; i < nbMentions; ++i) {
        const auto mention = static_cast<Mention>(i);
        const double pourcentage = 100.0 * repartition[i] / taille;
        std::cout << std::left << std::setw(14) << libelle(mention)
                  << " : " << std::setw(3) << repartition[i]
                  << " (" << pourcentage << " %)\n";
    }
    return EXIT_SUCCESS;
}