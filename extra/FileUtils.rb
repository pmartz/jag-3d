#!/usr/bin/env ruby
require 'optparse'
require 'ostruct'

class ArgvOptionsParser
   def self.parse(args)
      options = OpenStruct.new
      options.classname = []
      options.filenames = []
      options.namespace = ""
      options.path = ""
      options.add_copyright = true

      opts = OptionParser.new do |opts|
         opts.banner = "Usage: FileUtils.rb [options]"
         opts.separator " "
         
         opts.on("-u", "--update file(s)",  Array,
              "File to update copyright for") do |filename|
                    options.filenames = filename
         end

         opts.on_tail("-h", "--help", "Show this message") do
            puts opts
            exit
         end
      end

      opts.parse!
      options
   end
end

options = ArgvOptionsParser.parse(ARGV)
copyright = []
copyright << "/*************** <auto-copyright.rb BEGIN do not edit this line> **************\n"
copyright << " \n"
copyright << " Copyright 2012-2014 by Ames Laboratory and Skew Matrix Software, LLC\n"
copyright << " \n"
copyright << " Permission is granted to anyone to use this software for any purpose,\n"
copyright << " including commercial applications, and to alter it and redistribute it\n"
copyright << " freely, subject to the following restrictions:\n"
copyright << " \n"
copyright << "    1. The origin of this software must not be misrepresented; you must not\n"
copyright << "    claim that you wrote the original software. If you use this software\n"
copyright << "    in a product, an acknowledgment in the product documentation would be\n"
copyright << "    appreciated but is not required.\n"
copyright << " \n"
copyright << "    2. Altered source versions must be plainly marked as such, and must not be\n"
copyright << "    misrepresented as being the original software.\n"
copyright << " \n"
copyright << "    3. This notice may not be removed or altered from any source\n"
copyright << "    distribution.\n"
copyright << " \n"
copyright << " *************** <auto-copyright.rb END do not edit this line> ***************/\n"

puts options.filenames

ARGV.each do |file_name|
  if File.file? file_name
    file = File.new(file_name)
    lines = file.readlines
    file.close

    changes = false
    lines.each do |line|
      changes = true if line.include? "<auto-copyright.rb BEGIN"
    end
    
    if not changes
      no_header = ""
      no_header += file_name.to_s
      no_header += " does not contain a copyright header"
      puts no_header
    end

    # Don't write the file if there are no changes
    copyright_changing = false
    if changes
      file = File.new(file_name,'w')
      lines.each do |line|
        if line.include? "<auto-copyright.rb BEGIN"
          copyright_changing = true
          file_changing = "Changing: "
          file_changing += file_name.to_s
          puts file_changing
          copyright.each do |cp_line|
            file.write(cp_line)
          end
        end
        if not copyright_changing
          file.write(line)
        end
        copyright_changing = false if line.include? "<auto-copyright.rb END"
      end
      file.close
    end
  end
end

