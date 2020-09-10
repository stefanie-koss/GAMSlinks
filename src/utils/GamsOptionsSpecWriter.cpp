/** Copyright (C) GAMS Development and others 2009-2019
  * All Rights Reserved.
  * This code is published under the Eclipse Public License.
  *
  * @file GamsOptionsSpecWriter.cpp
  * @author Stefan Vigerske
  */

#include "GamsLinksConfig.h"
#include "GamsOptionsSpecWriter.hpp"

#include <cstdio>
#include <iostream>
#include <fstream>
#include <sstream>

std::string GamsOption::Value::toStringGams(
   GamsOption::Type type,
   bool             quotestr
) const
{
   std::stringstream s;
   switch( type )
   {
      case GamsOption::Type::BOOL:
         s << boolval;
         break;

      case GamsOption::Type::INTEGER:
         if( intval == -INT_MAX )
            s << "minint";
         else if( intval == INT_MAX )
            s << "maxint";
         else
            s << intval;
         break;

      case GamsOption::Type::REAL:
         if( realval == -DBL_MAX )
            s << "mindouble";
         else if( realval == DBL_MAX )
            s << "maxdouble";
         else
            s << realval;
         break;

      case GamsOption::Type::CHAR:
         if( quotestr )
            s << '"' << charval << '"';
         else
            s << charval;
         break;

      case GamsOption::Type::STRING:
         if( quotestr )
            s << '"' << GamsOptions::makeValidMarkdownString(stringval) << '"';
         else
            s << GamsOptions::makeValidMarkdownString(stringval);
         break;
   }
   return s.str();
}

std::string GamsOption::Value::toStringMarkdown(
   GamsOption::Type type,
   bool             doxygen
) const
{
   std::stringstream s;
   switch( type )
   {
      case GamsOption::Type::BOOL:
         s << boolval;
         break;

      case GamsOption::Type::INTEGER:
         if( intval == -INT_MAX )
            s << (doxygen ? "-&infin;" : "-infinity");
         else if( intval == INT_MAX )
            s << (doxygen ? "&infin;" : "infinity");
         else
            s << intval;
         break;

      case GamsOption::Type::REAL:
         if( realval == -DBL_MAX )
            s << (doxygen ? "-&infin;" : "-infinity");
         else if( realval == DBL_MAX )
            s << (doxygen ? "&infin;" : "infinity");
         else
            s << realval;
         break;

      case GamsOption::Type::CHAR:
         s << charval;
         break;

      case GamsOption::Type::STRING:
         s << GamsOptions::makeValidMarkdownString(stringval);
         break;
   }
   return s.str();
}

/// get string describing range of option in markdown
std::string GamsOption::getRangeMarkdown(
   bool doxygen
   ) const
{
   std::stringstream s;

   switch( type )
   {
      case GamsOption::Type::BOOL :
      {
         s << "boolean";
         break;
      }
      case GamsOption::Type::INTEGER :
      {
         s << "{";
         if( enumval.empty() )
         {
            s << minval.toStringMarkdown(type, doxygen);
            s << ", ..., ";
            s << maxval.toStringMarkdown(type, doxygen);
         }
         else
         {
            bool first = true;
            for( auto& e : enumval )
            {
               if( !first )
                  s << ", ";
               s << e.first.toStringMarkdown(type, doxygen);
               first = false;
            }
         }
         s << "}";
         break;
      }
      case GamsOption::Type::REAL :
      {
         if( minval.realval == -DBL_MAX && maxval.realval == DBL_MAX )
         {
            s << "real";
            break;
         }
         if( enumval.empty() )
         {
            s << "[";
            s << minval.toStringMarkdown(type, doxygen);
            s << ", ";
            s << maxval.toStringMarkdown(type, doxygen);
            s << "]";
         }
         else
         {
            s << "{";
            bool first = true;
            for( auto& e : enumval )
            {
               if( !first )
                  s << ", ";
               s << e.first.toStringMarkdown(type, doxygen);
               first = false;
            }
            s << "}";
         }
         break;
      }
      case GamsOption::Type::CHAR :
      {
         if( enumval.empty() )
         {
            s << "character";
         }
         else
         {
            bool first = true;
            for( auto& e : enumval )
            {
               if( !first )
                  s << ", ";
               s << e.first.toStringMarkdown(type, doxygen);
               first = false;
            }
         }
         break;
      }
      case GamsOption::Type::STRING :
      {
         if( enumval.empty() )
         {
            s << "string";
         }
         else
         {
            bool first = true;
            for( auto& e : enumval )
            {
               if( !first )
                  s << ", ";
               s << e.first.toStringMarkdown(type, doxygen);
               first = false;
            }
         }
         break;
      }
   }

   return s.str();
}

/// sorts options
void GamsOptions::finalize()
{
   options.sort();
   // TODO sort enum values?
}

/// replace problematic characters in identifier by underscore
static
std::string formatID(
   const std::string& origid
   )
{
   std::string id(origid);
   std::replace(id.begin(), id.end(), ' ', '_');
   std::replace(id.begin(), id.end(), '(', '_');
   std::replace(id.begin(), id.end(), ')', '_');
   std::replace(id.begin(), id.end(), '-', '_');
   std::replace(id.begin(), id.end(), '/', '_');
   return id;
}

void GamsOptions::writeGMS(
   bool shortdoc
   )
{
   // process or sort out options for GAMS (TODO do this on a copy of options?)
   // collect values
   std::set<std::string> values;
   for( std::list<GamsOption>::iterator o(options.begin()); o != options.end(); )
   {
      /* ignore options with number in beginning, because the GAMS options object cannot handle them so far */
      //      if( isdigit(name[0]) )
      //      {
      //         std::cerr << "Warning: Ignoring " << solver << " option " << name << " for GAMS options file." << std::endl;
      //         return;
      //      }
      if( o->name.length() > 63 )
      {
         std::cerr << "Skip writing option " << o->name << " because its name is too long for stupid GAMS." << std::endl;
         o = options.erase(o);
         continue;
      }

      if( o->shortdescr.length() >= 255 )
      {
         std::cerr << "Short description of option " << o->name << " too long for stupid GAMS. Moving to long description." << std::endl;
         if( !o->longdescr.empty() )
            o->longdescr = o->shortdescr + " " + o->longdescr;
         else
            o->longdescr = o->shortdescr;
         o->shortdescr.clear();
      }

      if( o->name.find(".") != std::string::npos && !o->longdescr.empty() )
      {
         std::cerr << "Cannot have long description because option " << o->name << " has a dot in the name and GAMS is stupid. Skipping long description." << std::endl;
         o->longdescr.clear();
      }

      /* replace all double quotes by single quotes */
      std::replace(o->shortdescr.begin(), o->shortdescr.end(), '"', '\'');

      switch( o->type )
      {
         case GamsOption::Type::BOOL:
         case GamsOption::Type::INTEGER:
         case GamsOption::Type::REAL:
            break;

         case GamsOption::Type::CHAR:
         {
            std::string str;
            str.push_back(o->defaultval.charval);
            values.insert(tolower(str));
            break;
         }

         case GamsOption::Type::STRING:
         {
            if( o->defaultval.stringval[0] != '\0' )
               values.insert(tolower(o->defaultval.stringval));
            break;
         }
      }

      /* collect enum values for values
       * update enum description
       */
      for( auto& e : o->enumval )
      {
         switch( o->type )
         {
            case GamsOption::Type::BOOL:
               values.insert(std::to_string(e.first.boolval));
               break;

            case GamsOption::Type::INTEGER:
               values.insert(std::to_string(e.first.intval));
               break;

            case GamsOption::Type::REAL:
               values.insert(std::to_string(e.first.realval));
               break;

            case GamsOption::Type::CHAR:
               values.insert(tolower(std::to_string(e.first.charval)));
               break;

            case GamsOption::Type::STRING:
               values.insert(tolower(e.first.stringval));
               break;
         }

         /* replace all double quotes by single quotes */
         std::replace(e.second.begin(), e.second.end(), '"', '\'');
         if( e.second.length() >= 255 )
         {
            size_t pos = e.second.rfind('.');
            if( pos < std::string::npos )
               e.second.erase(pos+1, e.second.length());
            else
               std::cerr << "Could not cut down description of enum value of parameter " << o->name << ": " << e.second << std::endl;
         }
      }

      ++o;
   }

   std::string filename = "opt" + tolower(solver) + ".gms";
   std::cout << "Writing " << filename << std::endl;
   std::ofstream f(filename.c_str());

   f << "$setglobal SEPARATOR \"" << separator << '"' << std::endl;
   if( stringquote == "\"" )
      f << "$setglobal STRINGQUOTE '\"'" << std::endl;
   else
      f << "$setglobal STRINGQUOTE \"" << stringquote << "\"" << std::endl;
   if( !eolchars.empty() )
      f << "$setglobal EOLCHAR \"" << eolchars << "\"" << std::endl;
   if( shortdoc )
      f << "$setglobal SHORTDOCONLY" << std::endl;
   f << "$onempty" << std::endl;

   f << "set e / 0*100 " << std::endl;
   for( int i = 0; i <= 100; ++i )
      values.erase(std::to_string(i));
   for( std::set<std::string>::iterator v(values.begin()); v != values.end(); ++v )
      f << "  '" << *v << "'" << std::endl;
   f << "/;" << std::endl;

   f << "set f / def Default, lo Lower Bound, up Upper Bound, ref Reference /;" << std::endl;
   f << "set t / I Integer, R Real, S String, B Binary /;" << std::endl;
   //f << "set m / %system.gamsopt% /;" << std::endl;

   f << "set g Option Groups /" << std::endl;
   for( std::map<std::string, std::string>::iterator g(groups.begin()); g != groups.end(); ++g )
      f << "  gr_" << formatID(g->first) << "   '" << (g->second.length() > 0 ? g->second : g->first) << "'" << std::endl;
   f << "/;" << std::endl;

   f << "set o Solver and Link Options with one-line description /" << std::endl;
   for( std::list<GamsOption>::iterator d(options.begin()); d != options.end(); ++d )
      f << "  '" << d->name << "'  \"" << makeValidMarkdownString(d->shortdescr) << '"' << std::endl;
   f << "/;" << std::endl;

   f << "$onembedded" << std::endl;
   f << "set optdata(g,o,t,f) /" << std::endl;
   bool havelongdescr = false;
   for( std::list<GamsOption>::iterator d(options.begin()); d != options.end(); ++d )
   {
      if( d->longdescr.length() > 0 )
         havelongdescr = true;

      f << "  gr_" << formatID(d->group) << ".'" << d->name << "'.";
      switch( d->type )
      {
         case GamsOption::Type::BOOL:
            f << "B.(def " << d->defaultval.boolval;
            break;

         case GamsOption::Type::INTEGER:
            f << "I.(def ";
            f << d->defaultval.toStringGams(d->type);
            if( d->minval.intval == -INT_MAX )
               f << ", lo minint";
            else if( d->minval.intval != 0 )
               f << ", lo " << d->minval.intval;
            if( d->maxval.intval !=  INT_MAX )
               f << ", up " << d->maxval.intval;
            break;

         case GamsOption::Type::REAL:
            f << "R.(def ";
            f << d->defaultval.toStringGams(d->type);
            if( d->minval.realval == -DBL_MAX )
               f << ", lo mindouble";
            else if( d->minval.realval != 0 )
               f << ", lo " << d->minval.realval;
            if( d->maxval.realval !=  DBL_MAX )
               f << ", up " << d->maxval.realval;
            break;

         case GamsOption::Type::CHAR:
            /* no character type in GAMS option files */
            f << "S.(def '" << d->defaultval.charval << '\'';
            break;

         case GamsOption::Type::STRING:
            f << "S.(def '" << makeValidMarkdownString(d->defaultval.stringval) << '\'';
            break;
      }
      if( d->refval >= -1 )
         f << ", ref " << d->refval;
      f << ")" << std::endl;
   }
   f << "/;" << std::endl;

   f << "set oe(o,e) /" << std::endl;
   for( std::list<GamsOption>::iterator d(options.begin()); d != options.end(); ++d )
   {
      for( GamsOption::EnumVals::iterator e(d->enumval.begin()); e != d->enumval.end(); ++e )
      {
         f << "  '" << d->name << "'.'";
         switch( d->type )
         {
            case GamsOption::Type::BOOL :
               f << e->first.boolval;
               break;
            case GamsOption::Type::INTEGER :
               f << e->first.intval;
               break;
            case GamsOption::Type::REAL :
               f << e->first.realval;
               break;
            case GamsOption::Type::CHAR :
               f << e->first.charval;
               break;
            case GamsOption::Type::STRING :
               f << e->first.stringval;
               break;
         }
         f << '\'';
         if( !e->second.empty() )
            f << "  \"" << makeValidMarkdownString(e->second) << '"';
         f << std::endl;
      }
   }
   f << "/;" << std::endl;

   f << "$onempty" << std::endl;

   f << "set odefault(o) /" << std::endl;
   for( std::list<GamsOption>::iterator d(options.begin()); d != options.end(); ++d )
   {
      if( d->defaultdescr.length() == 0 )
         continue;

      f << "  '" << d->name << "' '" << makeValidMarkdownString(d->defaultdescr) << '\'' << std::endl;
   }
   f << "/;" << std::endl;

   f << "set os(o,*) synonyms  /";
   for( std::list<GamsOption>::iterator d(options.begin()); d != options.end(); ++d )
      if( !d->synonyms.empty() )
         for( std::set<std::string>::const_iterator s(d->synonyms.begin()); s != d->synonyms.end(); ++s )
            f << std::endl << "  '" << d->name << "'.'" << *s << '\'';
   f << " /;" << std::endl
      << "set im immediates recognized  / EolFlag , Message /;" << std::endl   /* ???? */
      << "set strlist(o)        / /;" << std::endl
      << "set immediate(o,im)   / /;" << std::endl
      << "set multilist(o,o)    / /;" << std::endl
      << "set indicator(o)      / /;" << std::endl
      << "set dotoption(o)      / /;" << std::endl
      << "set dropdefaults(o)   / /;" << std::endl
      << "set deprecated(*,*)   / /;" << std::endl
      << "set oedepr(o,e)       / /;" << std::endl  /* deprecated enum options */
      << "set oep(o)            / /;" << std::endl  /* enum options for documentation only */
      << "set olink(o)          / /;" << std::endl
      //<< "set om(o,m)           / /;" << std::endl  /* option with GAMS synonym and GAMS initialization */
      << "$offempty" << std::endl;

   f.close();

   if( havelongdescr )
   {
      filename = "opt" + tolower(solver) + ".txt_";
      std::cout << "Writing " << filename << std::endl;
      f.open(filename.c_str());
      for( std::list<GamsOption>::iterator d(options.begin()); d != options.end(); ++d )
      {
         if( d->longdescr.length() == 0 )
            continue;
         f << d->name << std::endl;
         f << makeValidMarkdownString(d->longdescr) << std::endl;
         f << std::endl << std::endl;
      }

      f.close();

      std::string foldcall = "fold -s "+filename+" > opt" + tolower(solver) + ".txt";
      std::cout << "Folding " << filename << " to opt" << tolower(solver) << ".txt" << std::endl;
      system(foldcall.c_str());
      remove(filename.c_str());
   }
}

void GamsOptions::writeDef()
{
   std::string filename = "opt" + tolower(solver) + ".def";
   std::cout << "Writing " << filename << std::endl;
   std::ofstream f(filename.c_str());

   // write options
   bool havesynonym = false;
   for( auto& opt : options )
   {
      f << opt.name << ' ';

      // type
      switch( opt.type )
      {
         case GamsOption::Type::BOOL:
            f << "boolean";
            break;

         case GamsOption::Type::INTEGER:
            if( opt.enumval.empty() )
               f << "integer";
            else
               f << "enumint";
            break;

         case GamsOption::Type::REAL:
            f << "double";
            break;

         case GamsOption::Type::CHAR:
            if( opt.enumval.empty() )
               f << "string";
            else
               f << "enumstr";
            break;

         case GamsOption::Type::STRING:
            if( opt.enumval.empty() )
               f << "string";
            else
               f << "enumstr";
            break;
      }
      // ref
      f << ' ' << (opt.refval >= -1 ? opt.refval : 0) << ' ';

      // default
      f << opt.defaultval.toStringGams(opt.type, true);

      // lower, upper
      if( opt.type == GamsOption::Type::INTEGER || opt.type == GamsOption::Type::REAL )
      {
         f << ' ' << opt.minval.toStringGams(opt.type);
         f << ' ' << opt.maxval.toStringGams(opt.type);
      }

      // not hidden
      f << " 1";

      // group number
      f << ' ';
      size_t i = 1;
      for( std::map<std::string, std::string>::iterator it(groups.begin()); it != groups.end(); ++it )
      {
         if( it->first == opt.group )
         {
            f << i;
            break;
         }
         ++i;
      }

      // short description
      f << ' ' << opt.shortdescr;  // TODO is this allowed to be longer than 255 chars?
      f << std::endl;

      // enum values
      if( !opt.enumval.empty() && (opt.type != GamsOption::Type::BOOL && opt.type != GamsOption::Type::REAL) )
      {
         // std::sort(opt.enumval.begin(), opt.enumval.end(), GamsOption::EnumValCompare(opt.type));
         for( auto& eval : opt.enumval )
         {
            f << ' ' << eval.first.toStringGams(opt.type, true);

            // not hidden
            f << " 1";

            // short description
            if( !eval.second.empty() )
               f << ' ' << eval.second;   // TODO is this allowed to be longer than 255 chars?

            f << std::endl;
         }
      }

      if( !opt.synonyms.empty() )
         havesynonym = true;
   }

   // write synonyms
   if( havesynonym )
   {
      f << '*' << std::endl;
      f << "* synonym section" << std::endl;
      f << '*' << std::endl;
      for( auto& opt : options )
         for( auto& synonym : opt.synonyms )
            f << synonym << " synonym " << opt.name << std::endl;
   }

   // write indicators
   f << '*' << std::endl;
   f << "* indicator section" << std::endl;
   f << '*' << std::endl;
   if( !eolchars.empty() )
      f << "myeolchar EOLCOMM " << eolchars << std::endl;
   if( !separator.empty() )
      f << "indicator SEPARATOR " << '"' << separator << '"' << std::endl;
   f << "indicator STRINGQUOTE " << stringquote << std::endl;

   // write groups
   f << '*' << std::endl;
   f << "* Groups" << std::endl;
   f << '*' << std::endl;
   size_t i = 1;
   for( auto& group : groups )
   {
      f << "gr_" << formatID(group.first);
      f << " group ";
      f << i++;
      // not hidden
      f << " 1";
      f << ' ' << (group.second.empty() ? group.first : group.second);
      f << std::endl;
   }

   f.close();
}

void GamsOptions::writeMarkdown(
   const char* filename
   )
{
   assert(filename != NULL);

   std::cout << "Writing " << filename << std::endl;
   std::ofstream f(filename);

   f << "# " << solver << " options" << std::endl << std::endl;

   for( auto& group : groups )
   {
      f << "## " << (group.second.empty() ? group.first : group.second) << std::endl << std::endl;

      for( auto& opt : options )
      {
         if( opt.group != group.first )
            continue;

         f << "**" << opt.name << "**: " << makeValidMarkdownString(opt.shortdescr) << std::endl;
         if( !opt.longdescr.empty() )
            f << std::endl << "> " << makeValidMarkdownString(opt.longdescr) << std::endl;
         f << std::endl;
         if( opt.enumval.empty() )
         {
            f << "> Range: " << opt.getRangeMarkdown() << "  " << std::endl;

            if( opt.defaultdescr.empty() )
            {
               if( opt.type == GamsOption::Type::STRING && opt.defaultval.stringval[0] == '\0' )
                  f << "> Default: _empty_  " << std::endl;
               else
                  f << "> Default: " << opt.defaultval.toStringMarkdown(opt.type) << "  " << std::endl;
            }
         }
         else
         {
            f << "> Possible values:" << std::endl << std::endl;
            for( auto& e : opt.enumval )
            {
               bool isdefault;
               f << "> - ";
               switch( opt.type )
               {
                  case GamsOption::Type::BOOL :
                     f << e.first.boolval;
                     isdefault = e.first.boolval == opt.defaultval.boolval;
                     break;
                  case GamsOption::Type::INTEGER :
                     f << e.first.toStringMarkdown(opt.type);
                     isdefault = e.first.intval == opt.defaultval.intval;
                     break;
                  case GamsOption::Type::REAL :
                     f << e.first.toStringMarkdown(opt.type);
                     isdefault = e.first.realval == opt.defaultval.realval;
                     break;
                  case GamsOption::Type::CHAR :
                     f << e.first.charval;
                     isdefault = e.first.charval == opt.defaultval.charval;
                     break;
                  case GamsOption::Type::STRING :
                     f << e.first.toStringMarkdown(opt.type);
                     isdefault = strcmp(e.first.stringval, opt.defaultval.stringval) == 0;
                     break;
               }
               if( isdefault && opt.defaultdescr.empty() )
                  f << " (default)";
               if( !e.second.empty() )
                  f << ": " << makeValidMarkdownString(e.second);
               f << std::endl;
            }
         }
         if( !opt.defaultdescr.empty() )
         {
            f << "> Default: " << opt.defaultdescr << "  " << std::endl;
         }
         if( !opt.synonyms.empty() )
         {
            f << "> Synonyms:";
            for( auto& syn : opt.synonyms )
            {
               f << ' ' << syn;
            }
            f << std::endl;
         }
         f << std::endl;
      }
   }

   f.close();
}


void GamsOptions::writeDoxygen(
   bool shortdoc
   )
{
   std::string filename = "opt" + tolower(solver) + "_s.md";
   std::cout << "Writing " << filename << std::endl;
   std::ofstream f(filename.c_str());

   for( auto& group : groups )
   {
      f << std::endl;
      f << "\\subsection ";

      f << toupper(solver) << "_gr_" << formatID(group.first);
      f << ' ' << (group.second.empty() ? group.first : group.second) << std::endl;

      f << "| Option | Description | Default |" << std::endl;
      f << "|:-------|:------------| ------: |" << std::endl;

      for( auto& opt : options )
      {
         if( opt.group != group.first )
            continue;

         f << "| ";
         f << "\\anchor " << toupper(solver);
         f << formatID(opt.name);
         if( !shortdoc )
            f << "SHORTDOC \\ref " << toupper(solver) << formatID(opt.name) << " \"" << opt.name << "\"";
         else
            f << ' ' << opt.name;

         f << " | ";
         f << makeValidMarkdownString(opt.shortdescr);

         if( shortdoc )
         {
            if( !opt.longdescr.empty() )
               f << "<br/>" << opt.longdescr;

            if( opt.enumval.empty() )
            {
               // if( (opt.type == GamsOption::Type::INTEGER && (opt.minval.intval != 0 || opt.maxval.intval != INT_MAX)) ||
               //     (opt.type == GamsOption::Type::REAL && (opt.minval.realval != 0.0 || opt.maxval.realval != DBL_MAX)) )
               f << "<br/>Range: " << opt.getRangeMarkdown(true);
            }
            for( auto& e : opt.enumval )
            {
               f << "<br/>" << e.first.toStringMarkdown(opt.type, true);
               if( !e.second.empty() )
                  f << ": " << makeValidMarkdownString(e.second);
            }
            if( !opt.synonyms.empty() )
            {
               f << "<br/>Synonyms:";
               for( auto& syn : opt.synonyms )
               {
                  f << ' ' << syn;
               }
            }
         }

         f << " | ";
         if( opt.type != GamsOption::Type::STRING || opt.defaultval.stringval[0] != '\0' || !opt.defaultdescr.empty() )
         {
            if( opt.defaultdescr.empty() )
               f << opt.defaultval.toStringMarkdown(opt.type, true);
            else
               f << opt.defaultdescr;
         }
         f << '|' << std::endl;
      }
   }

   f.close();

   if( shortdoc )
      return;

   filename = "opt" + tolower(solver) + "_a.md";
   std::cout << "Writing " << filename << std::endl;
   f.open(filename.c_str());

   for( auto& opt : options )
   {
      f << "\\anchor " << toupper(solver) << formatID(opt.name) << std::endl;

      f << "<strong>" << opt.name << "</strong>";
      f << ": " << makeValidMarkdownString(opt.shortdescr) << std::endl;
      f << " \\ref " << toupper(solver) << formatID(opt.name) << "SHORTDOC \"&crarr;\"" << std::endl;

      f << "<blockquote>" << std::endl;

      if( !opt.longdescr.empty() )
         f << makeValidMarkdownString(opt.longdescr) << std::endl << std::endl;

      if( !opt.enumval.hasDescription() )
      {
         f << "Range: " << opt.getRangeMarkdown(true) << std::endl << std::endl;
      }
      else
      {
         f << "|value|meaning|" << std::endl;
         f << "|:----|:------|" << std::endl;
         for( auto& e : opt.enumval )
         {
            bool isdefault;
            f << "| " << e.first.toStringMarkdown(opt.type, true);
            f << " | " << makeValidMarkdownString(e.second) << "|" << std::endl;
         }
      }

      if( opt.defaultdescr.empty() )
      {
         if( opt.type == GamsOption::Type::STRING && opt.defaultval.stringval[0] == '\0' )
            f << "Default: _empty_" << std::endl;
         else
            f << "Default: " << opt.defaultval.toStringMarkdown(opt.type, true) << std::endl;
      }
      else
         f << "Default: " << opt.defaultdescr << std::endl;

      if( !opt.synonyms.empty() )
      {
         f << std::endl << "Synonym" << (opt.synonyms.size() > 1 ? "s" : "") << ':';
         for( auto& syn : opt.synonyms )
            f << ' ' << syn;
         f << std::endl;
      }

      f << "</blockquote>" << std::endl;
      f << std::endl;
   }
}
