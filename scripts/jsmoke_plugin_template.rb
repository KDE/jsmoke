=begin
This script was used to generate the QtScript plugin code. It takes two 
arguments - a module name and the full path of the 'smokedata.cpp' for the 
smoke lib that the plugin will use.

Example usage:

ruby ../jsmoke_project_template.rb QImageBlitz \
    /home/rdale/kde/build/4/kdebindings-qt4.6/smoke/qimageblitz/smokedata.cpp

It doesn't do a perfect job, and some hand hacking will be needed to get the 
generated code to compile.

Obviously it should be rewitten in QtScript at some point..

=end

module_name = ARGV[0]
# INCLUDE_DIR = "/usr/include/"
INCLUDE_DIR = "/opt/kde4/include/"
# INCLUDE_DIR = "#{ENV['KDEDIRS']}/include/"
# INCLUDE_AREA = module_name.downcase
INCLUDE_AREA = ""

plugin_program = <<EOS
#include <QtScript/QScriptExtensionPlugin>
#include <QtScript/QScriptValue>
#include <QtScript/QScriptEngine>

void qtscript_initialize_org_kde_#{module_name.downcase}_bindings(QScriptValue &);

class org_kde_#{module_name.downcase}_ScriptPlugin : public QScriptExtensionPlugin
{
public:
    QStringList keys() const;
    void initialize(const QString &key, QScriptEngine *engine);
};

QStringList org_kde_#{module_name.downcase}_ScriptPlugin::keys() const
{
    QStringList list;
    list << QLatin1String("jsmoke");
    list << QLatin1String("jsmoke.#{module_name.downcase}");
    return list;
}

void org_kde_#{module_name.downcase}_ScriptPlugin::initialize(const QString &key, QScriptEngine *engine)
{
    if (key == QLatin1String("jsmoke")) {
    } else if (key == QLatin1String("jsmoke.#{module_name.downcase}")) {
        QScriptValue extensionObject = engine->globalObject();
        qtscript_initialize_org_kde_#{module_name.downcase}_bindings(extensionObject);
    } else {
        Q_ASSERT_X(false, "org_kde_#{module_name.downcase}::initialize", qPrintable(key));
    }
}

Q_EXPORT_STATIC_PLUGIN(org_kde_#{module_name.downcase}_ScriptPlugin)
Q_EXPORT_PLUGIN2(qtscript_org_kde_#{module_name.downcase}, org_kde_#{module_name.downcase}_ScriptPlugin)
EOS

handlers_program = <<EOS
/*
 *   Copyright 2009 by Richard Dale <richard.j.dale@gmail.com>

 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU Library General Public License as
 *   published by the Free Software Foundation; either version 2, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details
 *
 *   You should have received a copy of the GNU Library General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include "marshall.h"
#include "global.h"
#include "marshallmacros.h"

EOS

main_program = <<EOS
/*
 *   Copyright 2009 by Richard Dale <richard.j.dale@gmail.com>

 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU Library General Public License as
 *   published by the Free Software Foundation; either version 2, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details
 *
 *   You should have received a copy of the GNU Library General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include <QtScript/QScriptValue>
#include <QtScript/QScriptEngine>

#include <global.h>
#include <marshall.h>

#include <smoke/#{module_name.downcase}_smoke.h>

namespace JSmoke {
extern Marshall::TypeHandler #{module_name}Handlers[];
extern void register#{module_name}Types(QScriptEngine * engine);  
}

void qtscript_initialize_org_kde_#{module_name.downcase}_bindings(QScriptValue& extensionObject)
{
static bool initialized = false;

    if (!initialized) {
        init_#{module_name.downcase}_Smoke();
        JSmoke::Module #{module_name.downcase}_module = { "#{module_name.downcase}", new JSmoke::Binding(#{module_name.downcase}_Smoke) };
        JSmoke::Global::modules[#{module_name.downcase}_Smoke] = #{module_name.downcase}_module;    
        JSmoke::Marshall::installHandlers(JSmoke::#{module_name}Handlers);
        initialized = true;
    }
    
    QScriptEngine* engine = extensionObject.engine();
    JSmoke::Global::initializeClasses(engine, #{module_name.downcase}_Smoke);
    JSmoke::register#{module_name}Types(engine);
    
    return;
    
}

// kate: space-indent on; indent-width 4; replace-tabs on; mixed-indent off;
EOS

cmakelists = <<EOS
include_directories(${CMAKE_SOURCE_DIR}/src ${QT_INCLUDES})

set(jsmoke#{module_name.downcase}_LIB_SRCS plugin.cpp #{module_name.downcase}.cpp #{module_name.downcase}handlers.cpp)

add_library(jsmoke#{module_name.downcase} MODULE ${jsmoke#{module_name.downcase}_LIB_SRCS})
target_link_libraries(  jsmoke#{module_name.downcase} 
                            ${QT_QTCORE_LIBRARY} 
                            ${QT_QTSCRIPT_LIBRARY} 
                            smokeqtcore 
                            smoke#{module_name.downcase} 
                            jsmokeruntime )

set_target_properties(jsmoke#{module_name.downcase} PROPERTIES PREFIX "")

install(TARGETS jsmoke#{module_name.downcase} DESTINATION ${QT_PLUGINS_DIR}/script )
EOS

top_cmakelists = <<EOS
OPTION(ENABLE_#{module_name.upcase}_JSMOKE "build #{module_name}JSmoke" OFF)

    if(ENABLE_#{module_name.upcase}_JSMOKE)
        add_subdirectory(#{module_name.downcase})
        set(JSMOKE_ENABLED "${JSMOKE_ENABLED}, #{module_name}")
    endif(ENABLE_#{module_name.upcase}_JSMOKE)

EOS

def find_includes(classname, namespace = nil)
  find_results = []
  if namespace
    find_command = "find #{INCLUDE_DIR}#{INCLUDE_AREA} -name \"*.h\" -exec egrep -l '(namespace|class).*#{namespace}' {} \\;"
    puts "find command: #{find_command}"
    
    find_inc = IO.popen(find_command, "r")
    find_results = find_inc.readlines
    if find_results.empty?
        return []
    end
  else
    find_command = "find #{INCLUDE_DIR}#{INCLUDE_AREA} -name \"*.h\" -exec egrep -l '(namespace|class).*#{classname}' {} \\;"
    puts find_command
    
    find_inc = IO.popen(find_command, "r")
    find_results = find_inc.readlines
    if find_results.empty?
        return []
    end
  end
  
#  puts find_command
  results = []
  grep_command = "grep -l 'class.*EXPORT.* #{classname}' #{find_results.join(' ').gsub(/\n/, '')}"
  puts grep_command
  
  inc = IO.popen(grep_command, "r")
  inc.readlines.each do |include_file|
    include_file.sub!(INCLUDE_DIR, '').sub!(/\n/, '')
#    puts "#include <#{include_file}>"
    results << "#include <#{include_file}>"
  end
  return results
end

in_smoke_types = false
smoke_lib_name = ""

lines_out = []
macro_calls = []
declare_metatypes = []
includes = []
handlers_table = []
register_types = []
register_pointer_types = []
in_smoke_types = false

File.open(ARGV[1]).readlines.each do |line|
  if line =~ /static Smoke::Type types\[\]/
    in_smoke_types = true
  end

  if in_smoke_types && line =~ /^\};/
    in_smoke_types = false
  end
  
  if in_smoke_types && (line =~ /List/ || line =~ /Vector/)
    line.sub!(/^\s*\{\s*"/, '')
    line.sub!(/^const /, '')
    line.sub!(/",.*/, '')
    lines_out << line
  end
end

result = []
lines_out.sort.uniq.each do |line|
    line.sub!(/\n/, '')
    if line =~ /^(QList|QVector|QLinkedList)(<(.*)>).*$/
      list_type = $1
      macro_name = 'DEF_CONTAINER_MARSHALLER'
      element_type = $3
      list_template = $2
      list_name = list_type + element_type.gsub(/::/, '').gsub(/\*/, '')
      macro_calls << "#{macro_name}(#{list_name}, #{list_type}#{list_template})"
      declare_metatypes << "Q_DECLARE_METATYPE(#{element_type})"
      declare_metatypes << "Q_DECLARE_METATYPE(#{list_type}#{list_template})"
      handlers_table << "    { \"#{line}\", marshall_#{list_name} },"
      if list_template =~ /\*>$/
          register_pointer_types << "    qScriptSmokeRegisterPointerSequenceMetaType<#{list_type}#{list_template} >(engine);"
      else
          register_types << "    qScriptSmokeRegisterSequenceMetaType<#{list_type}#{list_template} >(engine);"
      end
      
      if element_type =~ /^([^:]*)::([^:]*)$/
        find_includes($2, $1).each do |inc|
          includes << inc
        end
      else
        find_includes(element_type).each do |inc|
          includes << inc
        end
      end
      
    else
      puts "NO MATCH: #{line}"
    end
end

handlers_file = File.open("#{module_name.downcase}handlers.cpp", "w")
handlers_file.puts handlers_program

includes.sort.uniq.each do |line|
  handlers_file.puts line
end
handlers_file.puts ""

declare_metatypes.sort.uniq.each do |line|
  handlers_file.puts line
end

handlers_file.puts ""
handlers_file.puts "namespace JSmoke {"
handlers_file.puts ""

macro_calls.sort.uniq.each do |line|
  handlers_file.puts line
end

handlers_file.puts "\n"

handlers_file.puts "Marshall::TypeHandler #{module_name}Handlers[] = {"

handlers_table.sort.uniq.each do |line|
  handlers_file.puts line
end
handlers_file.puts "    { 0, 0 }"
handlers_file.puts "};"

handlers_file.puts ""
handlers_file.puts "void register#{module_name}Types(QScriptEngine * engine)"
handlers_file.puts "{"

register_types.sort.uniq.each do |line|
  handlers_file.puts line
end

handlers_file.puts ""

register_pointer_types.sort.uniq.each do |line|
  handlers_file.puts line
end

handlers_file.puts ""
handlers_file.puts "    return;"
handlers_file.puts "}"
handlers_file.puts ""
handlers_file.puts "}"
handlers_file.puts ""
handlers_file.puts "// kate: space-indent on; indent-width 4; replace-tabs on; mixed-indent off;"

plugin_program_file = File.open("plugin.cpp", "w")
plugin_program_file.puts plugin_program

main_program_file = File.open("#{module_name.downcase}.cpp", "w")
main_program_file.puts main_program

cmakelists_file = File.open("CMakeLists.txt", "w")
cmakelists_file.puts cmakelists

top_cmakelists_file = File.open("top_CMakeLists.txt", "w")
top_cmakelists_file.puts top_cmakelists
