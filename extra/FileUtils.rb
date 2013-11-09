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
         opts.separator ""
         
         opts.on("-u", "--update file(s)",
              "File to update copyright for") do |filename|
                    options.filenames << filename
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
copyright << " Copyright 2012-2014 by Ames Laboratory\n"
copyright << " \n"
copyright << " The MIT License (MIT)\n"
copyright << " \n"
copyright << " Permission is hereby granted, free of charge, to any person obtaining a copy\n"
copyright << " of this software and associated documentation files (the "Software"), to deal\n"
copyright << " in the Software without restriction, including without limitation the rights\n"
copyright << " to use, copy, modify, merge, publish, distribute, sublicense, and/or sell\n"
copyright << " copies of the Software, and to permit persons to whom the Software is\n"
copyright << " furnished to do so, subject to the following conditions:\n"
copyright << " \n"
copyright << " The above copyright notice and this permission notice shall be included in\n"
copyright << " all copies or substantial portions of the Software.\n"
copyright << " \n"
copyright << " THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR\n"
copyright << " IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,\n"
copyright << " FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE\n"
copyright << " AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER\n"
copyright << " LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,\n"
copyright << " OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN\n"
copyright << " THE SOFTWARE.\n"
copyright << " \n"
copyright << " *************** <auto-copyright.rb END do not edit this line> ***************/\n"

options.filenames.each do |file_name|
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

