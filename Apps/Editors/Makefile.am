bin_PROGRAMS+=object-editor

object_editor_SOURCES=
object_editor_CXXFLAGS=
object_editor_LDADD=

object_editor_MOC_SOURCES=
object_editor_MOC_SOURCES+=src/editors/object-editor/MainWindow.moc.cpp
object_editor_MOC_SOURCES+=src/editors/object-editor/EntityEditor.moc.cpp

BUILT_SOURCES+=src/editors/object-editor/MainWindow.ui.hpp
BUILT_SOURCES+=src/editors/object-editor/EntityEditor.ui.hpp
BUILT_SOURCES+=$(object_editor_MOC_SOURCES)

object_editor_SOURCES+=src/editors/object-editor/main.cpp
object_editor_SOURCES+=src/editors/object-editor/MainWindow.cpp
object_editor_SOURCES+=src/editors/object-editor/EntityEditor.cpp
object_editor_SOURCES+=$(object_editor_MOC_SOURCES)

object_editor_SOURCES+=$(engine_source_files)
object_editor_SOURCES+=src/framework/system/sdl.cpp

object_editor_CXXFLAGS+=$(AM_CPPFLAGS) $(QT_CFLAGS) $(qt5_extra_cxxflags)  $(SDL2_CFLAGS)

object_editor_LDADD+=$(editor_common_libs)


src/editors/object-editor/MainWindow.ui.hpp: src/editors/object-editor/MainWindow.ui
	$(UIC) $? -o $@

src/editors/object-editor/MainWindow.moc.cpp: src/editors/object-editor/MainWindow.hpp
	$(MOC) $? -o $@

src/editors/object-editor/EntityEditor.ui.hpp: src/editors/object-editor/EntityEditor.ui
	$(UIC) $? -o $@

src/editors/object-editor/EntityEditor.moc.cpp: src/editors/object-editor/EntityEditor.hpp
	$(MOC) $? -o $@

