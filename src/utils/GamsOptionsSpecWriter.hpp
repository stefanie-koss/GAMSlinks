/** Copyright (C) GAMS Development and others 2009-2019
  * All Rights Reserved.
  * This code is published under the Eclipse Public License.
  *
  * @file GamsOptionsSpecWriter.hpp
  * @author Stefan Vigerske
 */

#ifndef GAMSOPTIONSSPECWRITER_HPP_
#define GAMSOPTIONSSPECWRITER_HPP_

#include <string>
#include <vector>
#include <set>
#include <map>
#include <list>

#include <cstring>
#include <cassert>
#include <climits>
#include <cfloat>

#ifdef _MSC_VER
#ifndef strcasecmp
#define strcasecmp _stricmp
#endif
#endif

class GamsOption
{
public:
   enum Type
   {
      BOOL,
      INTEGER,
      REAL,
      CHAR,
      STRING
   };

   union Value
   {
      bool    boolval;
      int     intval;
      double  realval;
      char    charval;
      char*   stringval;

      Value()
      {
         memset(this, 0, sizeof(Value));
      }

      Value(
         bool val
         )
      : boolval(val)
      { }

      Value(
         int val
         )
      : intval(val)
      { }

      Value(
         double val
         )
      : realval(val)
      { }

      Value(
         char val
         )
      : charval(val)
      { }

      Value(
         const char* val
         )
      : stringval(val != NULL ? strdup(val) : NULL)
      { }

      Value(
         const std::string& val
         )
      : stringval(strdup(val.c_str()))
      { }

      Value& operator=(
         bool val
         )
      {
         boolval = val;
         return *this;
      }

      Value& operator=(
         int val
         )
      {
         intval = val;
         return *this;
      }

      Value& operator=(
         double val
         )
      {
         realval = val;
         return *this;
      }

      Value& operator=(
         char val
         )
      {
         charval = val;
         return *this;
      }

      Value& operator=(
         const char* val
         )
      {
         stringval = val != NULL ? strdup(val) : NULL;
         return *this;
      }

      Value& operator=(
         const std::string& val
         )
      {
         stringval = strdup(val.c_str());
         return *this;
      }

      bool operator==(
         bool val
         ) const
      {
         return boolval == val;
      }

      bool operator==(
         int val
         ) const
      {
         return intval == val;
      }

      bool operator==(
         double val
         ) const
      {
         return realval == val;
      }

      bool operator==(
         char val
         ) const
      {
         return charval == val;
      }

      bool operator==(
         const char* val
         ) const
      {
         if( stringval == val )
            return true;
         if( stringval == NULL || val == NULL )
            return false;
         return strcmp(stringval, val) == 0;
      }

      bool operator==(
         const std::string& val
         ) const
      {
         return operator==(val.c_str());
      }

      template <typename T>
      bool operator!=(
         T val
         ) const
      {
         return !operator==(val);
      }

      std::string toStringGams(
         GamsOption::Type type,
         bool             quotestr = false
         ) const;

      std::string toStringMarkdown(
         GamsOption::Type type,
         bool             doxygen = false
         ) const;
   };

   class ValueCompare
   {
   private:
      GamsOption::Type type;
   public:
      ValueCompare(
         GamsOption::Type type_
         )
      : type(type_)
      { }

      bool operator()(
         const GamsOption::Value& a,
         const GamsOption::Value& b
         ) const
      {
         switch( type )
         {
            case GamsOption::Type::BOOL :
               return a.boolval < b.boolval;
            case GamsOption::Type::INTEGER :
               return a.intval < b.intval;
            case GamsOption::Type::REAL :
               return a.realval < b.realval;
            case GamsOption::Type::CHAR:
               return a.charval < b.charval;
            case GamsOption::Type::STRING:
               return strcasecmp(a.stringval, b.stringval) < 0;
         }
         return 0;
      }
   };

   class EnumVals : public std::vector<std::pair<GamsOption::Value, std::string> >
   {
   public:
      template <typename T>
      void append(
         const T&           key,
         const std::string& descr = ""
         )
      {
         emplace_back(key, descr);
      }

      /// returns whether there exists an enum value with non-empty description
      bool hasDescription() const
      {
         for( auto& e : *this )
            if( !e.second.empty() )
               return true;
         return false;
      }

      Value getMinKey(
         GamsOption::Type type
         ) const;

      Value getMaxKey(
         GamsOption::Type type
         ) const;
   };

   class EnumValCompare
   {
   private:
      ValueCompare valcmp;
   public:
      EnumValCompare(
         GamsOption::Type type_
         )
      : valcmp(type_)
      { }

      bool operator()(
         const EnumVals::value_type& a,
         const EnumVals::value_type& b
         ) const
      {
         return valcmp(a.first, b.first);
      }
   };

   std::string        group;
   std::string        name;
   std::string        shortdescr;
   std::string        longdescr;
   std::string        defaultdescr;
   Type               type;
   Value              defaultval;
   Value              minval;
   Value              maxval;
   EnumVals           enumval;
   int                refval;
   std::set<std::string> synonyms;

   /// constructor for general option
   GamsOption(
      const std::string& name_,
      const std::string& shortdescr_,
      const std::string& longdescr_,
      Type               type_,
      Value              defaultval_,
      Value              minval_,
      Value              maxval_,
      const EnumVals&    enumval_,
      const std::string& defaultdescr_ = std::string(),
      int                refval_ = -2
   )
   : name(name_),
     shortdescr(shortdescr_),
     longdescr(longdescr_),
     defaultdescr(defaultdescr_),
     type(type_),
     defaultval(defaultval_),
     minval(minval_),
     maxval(maxval_),
     enumval(enumval_),
     refval(refval_)
   { }

   /// constructor for real-type option
   GamsOption(
      const std::string& name,
      const std::string& shortdescr,
      const std::string& longdescr,
      double             defaultval,
      double             minval,
      double             maxval,
      const std::string& defaultdescr = std::string(),
      int                refval = -2
   )
   : GamsOption(name, shortdescr, longdescr,
      GamsOption::Type::REAL,
      Value(defaultval), Value(minval), Value(maxval),
      GamsOption::EnumVals(),
      defaultdescr, refval)
   { }

   /// constructor for integer-type option
   GamsOption(
      const std::string& name,
      const std::string& shortdescr,
      const std::string& longdescr,
      int                defaultval,
      int                minval,
      int                maxval,
      const std::string& defaultdescr = std::string(),
      int                refval = -2
   )
   : GamsOption(name, shortdescr, longdescr,
      GamsOption::Type::INTEGER,
      Value(defaultval), Value(minval), Value(maxval),
      GamsOption::EnumVals(),
      defaultdescr, refval)
   { }

   /// constructor for enumerated integer-type option
   GamsOption(
      const std::string& name,
      const std::string& shortdescr,
      const std::string& longdescr,
      int                defaultval,
      const GamsOption::EnumVals& enumval,
      const std::string& defaultdescr = std::string(),
      int                refval = -2
   )
   : GamsOption(name, shortdescr, longdescr,
      GamsOption::Type::INTEGER,
      Value(defaultval), Value(-INT_MAX), Value(INT_MAX),
      enumval, defaultdescr, refval)
   { }

   /// constructor for bool-type option
   GamsOption(
      const std::string& name,
      const std::string& shortdescr,
      const std::string& longdescr,
      bool               defaultval,
      const std::string& defaultdescr = std::string(),
      int                refval = -2
   )
   : GamsOption(name, shortdescr, longdescr,
      GamsOption::Type::BOOL,
      Value(defaultval), Value(), Value(),
      GamsOption::EnumVals(),
      defaultdescr, refval)
   { }

   /// constructor for string-type option
   GamsOption(
      const std::string& name,
      const std::string& shortdescr,
      const std::string& longdescr,
      const std::string& defaultval,
      int                refval = -2
   )
   : GamsOption(name, shortdescr, longdescr,
      GamsOption::Type::STRING,
      Value(defaultval), Value(), Value(),
      GamsOption::EnumVals(),
      std::string(), refval)
   { }

   /// constructor for enumerated string-type option
   GamsOption(
      const std::string& name,
      const std::string& shortdescr,
      const std::string& longdescr,
      const std::string& defaultval,
      const GamsOption::EnumVals& enumval,
      const std::string& defaultdescr = std::string(),
      int                refval = -2
   )
   : GamsOption(name, shortdescr, longdescr,
         GamsOption::Type::STRING,
         Value(defaultval), Value(), Value(),
         enumval, defaultdescr, refval)
   { }

   ~GamsOption()
   {
      if( type == Type::STRING )
      {
         // free C-strings
         free(defaultval.stringval);
         for( auto& e : enumval )
         {
            free(e.first.stringval);
         }
      }
   }

   /// get string describing range of option in markdown
   std::string getRangeMarkdown(
      bool doxygen = false
      ) const;

   // comparison for sorting options by name
   bool operator<(
      const GamsOption& other
      ) const
   {
      return strcasecmp(name.c_str(), other.name.c_str()) < 0;
   }
};


class GamsOptions
{
private:
   std::list<GamsOption> options;
   std::map<std::string, std::string> groups;

   std::string           solver;
   std::string           curgroup;
   std::string           separator;
   std::string           stringquote;
   std::string           eolchars;

public:
   GamsOptions(
      const std::string& solver_
      )
   : solver(solver_),
     separator(" ")
   { }

   void setGroup(
      const std::string& group,
      const std::string& description = ""
      )
   {
      curgroup = group;
      groups[group] = description;
   }

   void setSeparator(
      const std::string& sepa
      )
   {
      separator = sepa;
   }

   void setStringQuote(
      const std::string& strquote
      )
   {
      stringquote = strquote;
   }

   void setEolChars(
      const std::string& eolchs
      )
   {
      eolchars = eolchs;
   }

   /// append option to options list, assign it to current group
   template<class... Args>
   GamsOption& collect(
      Args&&... args)
   {
      options.emplace_back(std::forward<Args>(args)...);
      options.back().group = curgroup;
      return options.back();
   }

   /// sorts options and groups
   void finalize();

   /// write options in .gms + .txt for GAMS mkopt scripts
   void writeGMS(
      bool shortdoc = false
      );

   /// write options .def file
   void writeDef();

   /// write options in Markdown
   void writeMarkdown();

   /// write options in Doxygen-flavored Markdown as used for GAMS docu
   void writeDoxygen(
      bool shortdoc = false
      );
};

#endif // GAMSOPTIONSSPECWRITER_HPP_
