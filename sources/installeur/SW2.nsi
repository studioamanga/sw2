;====================================================================
; Fichier inclus
;====================================================================

!include "MUI.nsh" ; Fichier inclus afin d'utiliser la "Modern UI"

;====================================================================
; Configuration
;====================================================================

Name "SW2"                                                ;Nom de l'installeur
OutFile "SW2.exe"                                         ;Nom du fichier � cr�er
InstallDir $PROGRAMFILES\StudioAMANgA\SW2                              ;R�pertoire par d�faut d'installation

!define MUI_NAME "SW2"
!define MUI_ICON "../../data/s-w2.ico"
!define MUI_UNICON "../../data/s-w2.ico"

!define MUI_WELCOMEFINISHPAGE_BITMAP "accueil.bmp"             ;Image au format BMP, � gauche de la fen�tre, de dimensions recommand�s 164*314
!define MUI_UNWELCOMEFINISHPAGE_BITMAP "accueil.bmp"
!define MUI_HEADERIMAGE                                        ;Utilisation d'une image dans l'en-t�te des fen�tres
!define MUI_HEADERIMAGE_BITMAP "coder.bmp"                     ;Image � utiliser dans l'en-t�te
!define MUI_HEADERIMAGE_RIGHT                                  ;Position l'image d'en-t�te sur la droite
!define MUI_BGCOLOR ffffff                                     ;Couleur de fond de l'en-t�te
!define MUI_COMPONENTSPAGE_SMALLDESC                           ;Description horizontale, des composants � installer
!define MUI_FINISHPAGE_SHOWREADME "S-W 2  Lisez-moi.htm"              ; Case � cocher pour voir le fichier "lisez-moi.txt"
!define MUI_FINISHPAGE_LINK "Visitez le site de Studio AMANgA"
!define MUI_FINISHPAGE_LINK_LOCATION "http://studioamanga.free.fr"

;--------------------------------------------------------------------
; Listes des pages que nous souhaitions utiliser
;--------------------------------------------------------------------
!insertmacro MUI_PAGE_WELCOME		;Accueil de l'installation
!insertmacro MUI_PAGE_COMPONENTS	;S�lection des composants � installer
!insertmacro MUI_PAGE_DIRECTORY         ;S�lection du r�pertoire d'installation
!insertmacro MUI_PAGE_INSTFILES         ;Copie des fichiers
!insertmacro MUI_PAGE_FINISH		;Fin de l'installation

!insertmacro MUI_UNPAGE_WELCOME         ;Accueil de la d�sinstallation
!insertmacro MUI_UNPAGE_CONFIRM         ;Confirmation de la d�sinstallation
!insertmacro MUI_UNPAGE_INSTFILES	;Suppression des fichiers
!insertmacro MUI_UNPAGE_FINISH          ;Fin de la d�sinstallation

!insertmacro MUI_LANGUAGE "French"	;Langue utiliser pour les textes de l'installer

;====================================================================
; Sections pour l'installation
;====================================================================

;--------------------------------------------------------------------
; Section permettant la copie des fichiers qui permettent de visualiser le tunnel
;--------------------------------------------------------------------

Section "SW2" DescProg
	SectionIn RO			;Indique que la section "SW2" doit �tre install�e obligatoirement.
  
	SetOutPath $INSTDIR		;R�pertoire de destination des fichiers
  	File ..\..\*.*			;Localisation des fichiers � copier

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
	;R�pertoire de destination des fichiers
	SetOutPath $INSTDIR\sources\installeur
  	;Localisation des fichiers � copier
	File ..\..\sources\installeur\SW2.nsi
	File ..\..\sources\installeur\coder.bmp
	File ..\..\sources\installeur\accueil.bmp

	SetOutPath $INSTDIR\sources
	File ..\..\sources\*.*

	;Cr�ation du r�pertoire "SW2"
	CreateDirectory "$SMPROGRAMS\SW2"
	SetOutPath $INSTDIR
SectionEnd

;--------------------------------------------------------------------
; Section permettant la cr�ation de raccourcis dans le menu principal
;--------------------------------------------------------------------

Section "Raccourcis" DescRaccourcis
	;Cr�ation du r�pertoire "SW2"
	CreateDirectory "$SMPROGRAMS\SW2"

	SetOutPath $INSTDIR
	;Cr�ation des raccourcis dans le menu d�marrer
	;Liste des paramettres : 
	;"$SMPROGRAMS\SW2\SW2.lnk" => Chemin du lien dans le menu d�marrer
	;"$INSTDIR\SW2.exe" => Cible du lien
	;"" => Param�tres pass�s � l'ex�cutable lors de l'utilisation du lien
	;"$INSTDIR\data\skr.ico" => ic�ne du lien
	CreateShortCut "$SMPROGRAMS\SW2\SW2.lnk" "$INSTDIR\SW2.exe" "" "$INSTDIR\data\s-w2.ico"
	CreateShortCut "$SMPROGRAMS\SW2\Uninstall.lnk" "$INSTDIR\uninstall.exe" "" "$INSTDIR\uninstall.exe"
SectionEnd

;--------------------------------------------------------------------
; Descriptions des sections
;--------------------------------------------------------------------

;Textes qui sont affich�s lorsque l'utilisateur passe le curseur de sa souris sur un composant � cocher
LangString DESC_DescProg ${LANG_FRENCH}	"Fichiers n�cessaire � l'ex�cution du jeu."
LangString DESC_DescRaccourcis ${LANG_FRENCH}	"Cr�er des raccourcis dans le menu d�marrer."
LangString DESC_DescSources ${LANG_FRENCH}	"Code source de SW2 comprenant le code C++ ainsi que le script de l'installer NSIS."

!insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
	!insertmacro MUI_DESCRIPTION_TEXT ${DescProg} $(DESC_DescProg)
	!insertmacro MUI_DESCRIPTION_TEXT ${DescRaccourcis} $(DESC_DescRaccourcis)
	!insertmacro MUI_DESCRIPTION_TEXT ${DescSources} $(DESC_DescSources)
!insertmacro MUI_FUNCTION_DESCRIPTION_END
 
;====================================================================
; Sections pour la d�installation
;====================================================================

Section "Uninstall"
	;Suppression de l�entr�e du programme de la base de registre de Windows
	DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\SW2"

	;Suppression des raccourcis
	Delete $SMPROGRAMS\SW2\*.*

	;Suppression des r�pertoires dans le menu d�marrer
	RMDir $SMPROGRAMS\SW2
	;Suppression de tous les r�pertoires contenus dans le r�pertoire d'installation ainsi que le r�pertoire d'installation lui-m�me
	RMDir /r $INSTDIR
SectionEnd

