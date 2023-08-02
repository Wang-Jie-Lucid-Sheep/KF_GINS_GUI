#-------------------------------------------------
#
# Project created by QtCreator 2023-07-26T14:58:29
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = KF_GINS
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    config_view.cpp \
    fileio/fileloader.cc \
    fileio/filesaver.cc \
    kf-gins/gi_engine.cpp \
    kf-gins/insmech.cpp \
    draw_result_view.cpp \
    draw_error_view.cpp \
    kf_gins.cpp

HEADERS += \
        mainwindow.h \
    config_view.h \
    common/angle.h \
    common/earth.h \
    common/logging.h \
    common/rotation.h \
    common/types.h \
    kf-gins/gi_engine.h \
    kf-gins/insmech.h \
    kf-gins/kf_gins_types.h \
    fileio/filebase.h \
    fileio/fileloader.h \
    fileio/filesaver.h \
    fileio/gnssfileloader.h \
    fileio/imufileloader.h \
    draw_result_view.h \
    draw_error_view.h

INCLUDEPATH += \
    C:\Users\LENOVO\Documents\KF_GINS\ThirdParty\eigen-3.3.9 \
    C:\Users\LENOVO\Documents\KF_GINS\ThirdParty\abseil-cpp-20220623.1 \
    C:\Users\LENOVO\Documents\KF_GINS\ThirdParty\yaml-cpp-0.7.0\include \
    C:\Users\LENOVO\Documents\KF_GINS\ThirdParty\Pre-built.2\include \
    C:\Users\LENOVO\Documents\KF_GINS\lib

LIBS += \
    $$PWD/lib/yaml-cpp.lib \
    $$PWD/lib/absl_bad_any_cast_impl.lib \
    $$PWD/lib/absl_bad_optional_access.lib \
    $$PWD/lib/absl_bad_variant_access.lib \
    $$PWD/lib/absl_base.lib \
    $$PWD/lib/absl_city.lib \
    $$PWD/lib/absl_civil_time.lib \
    $$PWD/lib/absl_cord.lib \
    $$PWD/lib/absl_cord_internal.lib \
    $$PWD/lib/absl_cordz_functions.lib \
    $$PWD/lib/absl_cordz_handle.lib \
    $$PWD/lib/absl_cordz_info.lib \
    $$PWD/lib/absl_cordz_sample_token.lib \
    $$PWD/lib/absl_debugging_internal.lib \
    $$PWD/lib/absl_demangle_internal.lib \
    $$PWD/lib/absl_examine_stack.lib \
    $$PWD/lib/absl_exponential_biased.lib \
    $$PWD/lib/absl_failure_signal_handler.lib \
    $$PWD/lib/absl_flags.lib \
    $$PWD/lib/absl_flags_commandlineflag.lib \
    $$PWD/lib/absl_flags_commandlineflag_internal.lib \
    $$PWD/lib/absl_flags_config.lib \
    $$PWD/lib/absl_flags_internal.lib \
    $$PWD/lib/absl_flags_marshalling.lib \
    $$PWD/lib/absl_flags_parse.lib \
    $$PWD/lib/absl_flags_private_handle_accessor.lib \
    $$PWD/lib/absl_flags_program_name.lib \
    $$PWD/lib/absl_flags_reflection.lib \
    $$PWD/lib/absl_flags_usage.lib \
    $$PWD/lib/absl_flags_usage_internal.lib \
    $$PWD/lib/absl_graphcycles_internal.lib \
    $$PWD/lib/absl_hash.lib \
    $$PWD/lib/absl_hashtablez_sampler.lib \
    $$PWD/lib/absl_int128.lib \
    $$PWD/lib/absl_leak_check.lib \
    $$PWD/lib/absl_log_severity.lib \
    $$PWD/lib/absl_low_level_hash.lib \
    $$PWD/lib/absl_malloc_internal.lib \
    $$PWD/lib/absl_periodic_sampler.lib \
    $$PWD/lib/absl_random_distributions.lib \
    $$PWD/lib/absl_random_internal_distribution_test_util.lib \
    $$PWD/lib/absl_random_internal_platform.lib \
    $$PWD/lib/absl_random_internal_pool_urbg.lib \
    $$PWD/lib/absl_random_internal_randen.lib \
    $$PWD/lib/absl_random_internal_randen_hwaes.lib \
    $$PWD/lib/absl_random_internal_randen_hwaes_impl.lib \
    $$PWD/lib/absl_random_internal_randen_slow.lib \
    $$PWD/lib/absl_random_internal_seed_material.lib \
    $$PWD/lib/absl_random_seed_gen_exception.lib \
    $$PWD/lib/absl_random_seed_sequences.lib \
    $$PWD/lib/absl_raw_hash_set.lib \
    $$PWD/lib/absl_raw_logging_internal.lib \
    $$PWD/lib/absl_scoped_set_env.lib \
    $$PWD/lib/absl_spinlock_wait.lib \
    $$PWD/lib/absl_stacktrace.lib \
    $$PWD/lib/absl_status.lib \
    $$PWD/lib/absl_statusor.lib \
    $$PWD/lib/absl_str_format_internal.lib \
    $$PWD/lib/absl_strerror.lib \
    $$PWD/lib/absl_strings.lib \
    $$PWD/lib/absl_strings_internal.lib \
    $$PWD/lib/absl_symbolize.lib \
    $$PWD/lib/absl_synchronization.lib \
    $$PWD/lib/absl_throw_delegate.lib \
    $$PWD/lib/absl_time.lib \
    $$PWD/lib/absl_time_zone.lib \

FORMS += \
        mainwindow.ui \
    config_view.ui \
    draw_result_view.ui \
    draw_error_view.ui

RESOURCES += \
    resource.qrc

DISTFILES += \
    logo.rc

RC_FILE += logo.rc

RC_ICONS = ICON.ico
