;====================================================================
; Fichier inclus
;====================================================================

!include "MUI.nsh" ; Fichier inclus afin d'utiliser la "Modern UI"

;====================================================================
; Configuration
;====================================================================

Name "SW2"                                                ;Nom de l'installeur
OutFile "SW2.exe"                                         ;Nom du fichier à créer
InstallDir $PROGRAMFILES\StudioAMANgA\SW2                              ;Répertoire par défaut d'installation

!define MUI_NAME "SW2"
!define MUI_ICON "../../data/s-w2.ico"
!define MUI_UNICON "../../data/s-w2.ico"

!define MUI_WELCOMEFINISHPAGE_BITMAP "accueil.bmp"             ;Image au format BMP, à gauche de la fenêtre, de dimensions recommandés 164*314
!define MUI_UNWELCOMEFINISHPAGE_BITMAP "accueil.bmp"
!define MUI_HEADERIMAGE                                        ;Utilisation d'une image dans l'en-tête des fenêtres
!define MUI_HEADERIMAGE_BITMAP "coder.bmp"                     ;Image à utiliser dans l'en-tête
!define MUI_HEADERIMAGE_RIGHT                                  ;Position l'image d'en-tête sur la droite
!define MUI_BGCOLOR ffffff                                     ;Couleur de fond de l'en-tête
!define MUI_COMPONENTSPAGE_SMALLDESC                           ;Description horizontale, des composants à installer
!define MUI_FINISHPAGE_SHOWREADME "S-W 2  Lisez-moi.htm"              ; Case à cocher pour voir le fichier "lisez-moi.txt"
!define MUI_FINISHPAGE_LINK "Visitez le site de Studio AMANgA"
!define MUI_FINISHPAGE_LINK_LOCATION "http://studioamanga.free.fr"

;--------------------------------------------------------------------
; Listes des pages que nous souhaitions utiliser
;--------------------------------------------------------------------
!insertmacro MUI_PAGE_WELCOME		;Accueil de l'installation
!insertmacro MUI_PAGE_COMPONENTS	;Sélection des composants à installer
!insertmacro MUI_PAGE_DIRECTORY         ;Sélection du répertoire d'installation
!insertmacro MUI_PAGE_INSTFILES         ;Copie des fichiers
!insertmacro MUI_PAGE_FINISH		;Fin de l'installation

!insertmacro MUI_UNPAGE_WELCOME         ;Accueil de la désinstallation
!insertmacro MUI_UNPAGE_CONFIRM         ;Confirmation de la désinstallation
!insertmacro MUI_UNPAGE_INSTFILES	;Suppression des fichiers
!insertmacro MUI_UNPAGE_FINISH          ;Fin de la désinstallation

!insertmacro MUI_LANGUAGE "French"	;Langue utiliser pour les textes de l'installer

;====================================================================
; Sections pour l'installation
;====================================================================

;--------------------------------------------------------------------
; Section permettant la copie des fichiers qui permettent de visualiser le tunnel
;--------------------------------------------------------------------

Section "SW2" DescProg
	SectionIn RO			;Indique que la section "SW2" doit être installée obligatoirement.
  
	SetOutPath $INSTDIR		;Répertoire de destination des fichiers
  	File ..\..\*.*			;Localisation des fichiers à copier

	SetOutPath $INSTDIR\data
	File ..\..\data\*.*

	SetOutPath $INSTDIR\RM-data
	File ..\..\RM-data\*.*

	;Pour inscrire le programme dans le menu "ajout/suppression de programme" de Windows
	WriteRegExpandStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\SW2" "UninstallString" '"$INSTDIR\uninstall.exe"'
	WriteRegExpandStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\SW2" "InstallLocation" "$INSTDIR"
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\SW2" "DisplayName" "SW2"
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\SW2" "DisplayIcon" "$INSTDIR\data\s-w2.ico,0"
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\SW2" "URLInfoAbout" "http://studioamanga.free.fr"
	WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\SW2" "NoModify" "1"
	WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\SW2" "NoRepair" "1"
	WriteUninstaller "uninstall.exe"
SectionEnd

;--------------------------------------------------------------------
; Section permettant la copie du code source
;--------------------------------------------------------------------

Section "Sources" DescSources
	;Répertoire de destination des fichiers
	SetOutPath $INSTDIR\sources\installeur
  	;Localisation des fichiers à copier
	File ..\..\sources\installeur\SW2.nsi
	File ..\..\sources\installeur\coder.bmp
	File ..\..\sources\installeur\accueil.bmp

	SetOutPath $INSTDIR\sources
	File ..\..\sources\*.*

	;Création du répertoire "SW2"
	CreateDirectory "$SMPROGRAMS\SW2"
	SetOutPath $INSTDIR
SectionEnd

;--------------------------------------------------------------------
; Section permettant la création de raccourcis dans le menu principal
;--------------------------------------------------------------------

Section "Raccourcis" DescRaccourcis
	;Création du répertoire "SW2"
	CreateDirectory "$SMPROGRAMS\SW2"

	SetOutPath $INSTDIR
	;Création des raccourcis dans le menu démarrer
	;Liste des paramettres : 
	;"$SMPROGRAMS\SW2\SW2.lnk" => Chemin du lien dans le menu démarrer
	;"$INSTDIR\SW2.exe" => Cible du lien
	;"" => Paramètres passés à l'exécutable lors de l'utilisation du lien
	;"$INSTDIR\data\skr.ico" => icône du lien
	CreateShortCut "$SMPROGRAMS\SW2\SW2.lnk" "$INSTDIR\SW2.exe" "" "$INSTDIR\data\s-w2.ico"
	CreateShortCut "$SMPROGRAMS\SW2\Uninstall.lnk" "$INSTDIR\uninstall.exe" "" "$INSTDIR\uninstall.exe"
SectionEnd

;--------------------------------------------------------------------
; Descriptions des sections
;--------------------------------------------------------------------

;Textes qui sont affichés lorsque l'utilisateur passe le curseur de sa souris sur un composant à cocher
LangString DESC_DescProg ${LANG_FRENCH}	"Fichiers nécessaire à l'exécution du jeu."
LangString DESC_DescRaccourcis ${LANG_FRENCH}	"Créer des raccourcis dans le menu démarrer."
LangString DESC_DescSources ${LANG_FRENCH}	"Code source de SW2 comprenant le code C++ ainsi que le script de l'installer NSIS."

!insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
	!insertmacro MUI_DESCRIPTION_TEXT ${DescProg} $(DESC_DescProg)
	!insertmacro MUI_DESCRIPTION_TEXT ${DescRaccourcis} $(DESC_DescRaccourcis)
	!insertmacro MUI_DESCRIPTION_TEXT ${DescSources} $(DESC_DescSources)
!insertmacro MUI_FUNCTION_DESCRIPTION_END
 
;====================================================================
; Sections pour la déinstallation
;====================================================================

Section "Uninstall"
	;Suppression de l’entrée du programme de la base de registre de Windows
	DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\SW2"

	;Suppression des raccourcis
	Delete $SMPROGRAMS\SW2\*.*

	;Suppression des répertoires dans le menu démarrer
	RMDir $SMPROGRAMS\SW2
	;Suppression de tous les répertoires contenus dans le répertoire d'installation ainsi que le répertoire d'installation lui-même
	RMDir /r $INSTDIR
SectionEnd

