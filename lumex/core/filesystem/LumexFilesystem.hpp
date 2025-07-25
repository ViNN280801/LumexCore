#ifndef LUMEX_FILESYSTEM_HPP
#define LUMEX_FILESYSTEM_HPP

#include "lumex/LumexExport.hpp"
#include "lumex/core/utility/LumexUtility"

#include <cstdint>
#include <ctime>
#include <memory>
#include <ostream>
#include <string>
#include <vector>

/**
 * @file LumexFilesystem.hpp
 * @brief Cross-platform, header-only replacement for <filesystem> (C++17)
 * written in pure C++11.
 *
 * The design goals are:
 *   1. 100 % C++11-conformance – no compiler extensions, RTTI, or exceptions
 * required.
 *   2. No dynamic memory allocation inside core operations unless unavoidable.
 *   3. Zero-exception guarantee – all APIs return `FilesystemResult<T>` instead
 * of throwing.
 *   4. SOLID-compliant, header-only implementation with clean separation of
 * concerns.
 *   5. Thread-safety for read-only operations (stat, exists, etc.).
 *
 *     @code
 *     using Lumex::Path;
 *     using Lumex::Filesystem;
 *
 *     // create tree recursively
 *     Filesystem::create_directories(Path("sandbox/dir/sub"));
 *
 *     // iterate
 *     for (Lumex::DirectoryEntry const& e :
 * Filesystem::directory_contents(Path("sandbox"))) { std::cout <<
 * e.path().string() << std::endl;
 *     }
 *     @endcode
 */

// C++11 compatible nested namespaces
namespace Lumex // NOLINT(modernize-concat-nested-namespaces)
{
  namespace Core
  {
    namespace Filesystem
    {

      // 116444736000000000ULL - count of intervals between 1601 and 1970
      // If subtract this number from ulInt.QuadPart (which contains the number of
      // 100-nanosecond intervals since 1601) we get the number of 100-nanosecond
      // intervals since 1970.

      // 10000000ULL - count of 100-nanosecond interval in 1 second
      // Since std::time_t is measured in seconds, and ulInt.QuadPart (after
      // subtraction) contains the number of 100-nanosecond intervals, dividing by
      // 10000000ULL converts these 100-nanosecond intervals into seconds.
      constexpr unsigned long long KDEFAULT_WINDOWS_FILETIME_TO_UNIX_EPOCH_INTERVALS = 116444736000000000ULL;
      constexpr unsigned long long KDEFAULT_HUNDRED_NANOSECONDS_PER_SECOND           = 10000000ULL;

      // Forward declarations
      class Path;
      class DirectoryEntry;
      class DirectoryIterator;
      class FileStatus;
      class SpaceInfo;

      /**
       * @brief File type enumeration compatible with std::filesystem
       */
      enum class FileType : std::int8_t
      {
        not_found = -1,
        none      = 0,
        regular   = 1,
        directory = 2,
        symlink   = 3,
        block     = 4,
        character = 5,
        fifo      = 6,
        socket    = 7,
        unknown   = 8
      };

      /**
       * @brief File permissions compatible with POSIX
       */
      enum class Perms : std::uint16_t
      {
        none         = 0,
        owner_read   = 0400,
        owner_write  = 0200,
        owner_exec   = 0100,
        owner_all    = 0700,
        group_read   = 040,
        group_write  = 020,
        group_exec   = 010,
        group_all    = 070,
        others_read  = 04,
        others_write = 02,
        others_exec  = 01,
        others_all   = 07,
        all          = 0777,
        set_uid      = 04000,
        set_gid      = 02000,
        sticky_bit   = 01000,
        mask         = 07777,
        unknown      = 0xFFFF
      };

      // Enable bitwise operations for Perms
      inline Perms
      operator|(Perms lhs, Perms rhs)
      {
        return static_cast<Perms>(static_cast<int>(lhs) | static_cast<int>(rhs));
      }

      inline Perms
      operator&(Perms lhs, Perms rhs)
      {
        return static_cast<Perms>(static_cast<int>(lhs) & static_cast<int>(rhs));
      }

      inline Perms
      operator^(Perms lhs, Perms rhs)
      {
        return static_cast<Perms>(static_cast<int>(lhs) ^ static_cast<int>(rhs));
      }

      inline Perms
      operator~(Perms perms)
      {
        return static_cast<Perms>(~static_cast<int>(perms));
      }

      inline Perms &
      operator|=(Perms &lhs, Perms rhs)
      {
        return lhs = lhs | rhs;
      }

      inline Perms &
      operator&=(Perms &lhs, Perms rhs)
      {
        return lhs = lhs & rhs;
      }

      inline Perms &
      operator^=(Perms &lhs, Perms rhs)
      {
        return lhs = lhs ^ rhs;
      }

      /**
       * @brief Result type for filesystem operations with error handling
       * @tparam T The success value type
       */
      template <typename T> class FilesystemResult
      {
      public:
        FilesystemResult() : m_value(), m_success(true), m_error_code(0) {}

        // –– success factory
        static FilesystemResult<T>
        ok(T value)
        {
          return FilesystemResult<T>(std::move(value), /*success=*/true, /*err=*/0);
        }

        // –– error factory (with optional default)
        static FilesystemResult<T>
        err(int error_code, T default_value = T{})
        {
          return FilesystemResult<T>(std::move(default_value),
                                     /*success=*/false,
                                     /*err=*/error_code);
        }

        // Accessors
        T const &
        value() const
        {
          return m_value;
        }
        T &
        value()
        {
          return m_value;
        }
        bool
        success() const
        {
          return m_success;
        }
        int
        error_code() const
        {
          return m_error_code;
        }

        // Implicit bool conversion
        operator bool() const { return m_success; }

        // Get value or default
        T const &
        value_or(T const &default_value) const
        {
          T default_value_copy = default_value; // avoiding (bugprone-return-const-ref-from-parameter)
          return m_success ? m_value : default_value_copy;
        }

      private:
        // hidden ctor used by our factories
        FilesystemResult(T value, bool success, int error_code)
            : m_value(std::move(value)), m_success(success), m_error_code(error_code)
        {}

        T m_value;
        bool m_success;
        int m_error_code;
      };

      // Specialization for void (operations with no return value)
      template <> class FilesystemResult<void>
      {
      public:
        FilesystemResult() : m_success(true), m_error_code(0) {}

        // –– success factory
        static FilesystemResult<void>
        ok()
        {
          return FilesystemResult<void>(/*success=*/true, /*error_code=*/0);
        }

        // –– error factory
        static FilesystemResult<void>
        err(int error_code)
        {
          return FilesystemResult<void>(/*success=*/false, /*error_code=*/error_code);
        }

        bool
        success() const
        {
          return m_success;
        }
        int
        error_code() const
        {
          return m_error_code;
        }
        operator bool() const { return m_success; }

      private:
        // hidden ctor used by ok()/err()
        FilesystemResult(bool success, int error_code) : m_success(success), m_error_code(error_code) {}

        bool m_success;
        int m_error_code;
      };

      /**
       * @brief Cross-platform path representation and manipulation
       * @details Handles platform-specific path formats and character encodings
       *          internally while providing a unified API
       */
      class LUMEX_API Path
      {
      public:
        // Type aliases
        using string_type = std::string;
        using value_type  = char;
        static value_type const preferred_separator =
#if LUMEX_OS_WINDOWS
          '\\';
#else
          '/';
#endif

        // Constructors
        Path() : m_path(".") {} // Initialize to "." instead of empty
        Path(string_type source);
        Path(char const *source);
        Path(Path const &other)            = default;
        Path &operator=(Path const &other) = default;
        ~Path()                            = default;

        // C++11 move semantics
        Path(Path &&other) noexcept : m_path(std::move(other.m_path))
        {
          if(other.m_path.empty()) other.m_path = "."; // Ensure moved-from object is valid
        }
        Path &
        operator=(Path &&other) noexcept
        {
          if(this != std::addressof(other))
          {
            m_path = std::move(other.m_path);
            if(other.m_path.empty()) other.m_path = "."; // Ensure moved-from object is valid
          }
          return *this;
        }

        // Concatenation
        Path &operator/=(Path const &);
        Path &operator/=(string_type const &);
        Path &operator/=(char const *);

        Path &operator+=(Path const &);
        Path &operator+=(string_type const &);
        Path &operator+=(char const *);
        Path &operator+=(value_type);

        // Modifiers
        void
        clear()
        {
          m_path.clear();
        }
        Path &make_preferred();
        Path &remove_filename();
        Path &replace_filename(char const *filename);
        Path &replace_filename(std::string const &filename);
        Path &replace_filename(Path const &replacement);
        Path &replace_extension(char const *ext);
        Path &replace_extension(std::string const &ext);
        Path &replace_extension(Path const &ext = Path());
        void
        swap(Path &other) noexcept
        {
          m_path.swap(other.m_path);
        }

        // Native format observers
        string_type const &
        native() const
        {
          return m_path;
        }
        value_type const *
        c_str() const
        {
          return m_path.c_str();
        }
        operator string_type() const { return m_path; }

        // Generic format observers
        string_type
        string() const
        {
          return m_path;
        }
        std::wstring wstring() const;
        string_type
        u8string() const
        {
          return string();
        }

        // Decomposition
        Path root_name() const;
        Path root_directory() const;
        Path root_path() const;
        Path relative_path() const;
        Path parent_path() const;
        Path filename() const;
        Path stem() const;
        Path extension() const;

        // Queries
        bool
        empty() const
        {
          return m_path.empty();
        }
        bool has_root_name() const;
        bool has_root_directory() const;
        bool has_root_path() const;
        bool has_relative_path() const;
        bool has_parent_path() const;
        bool has_filename() const;
        bool has_stem() const;
        bool has_extension() const;
        bool is_absolute() const;
        bool
        is_relative() const
        {
          return !is_absolute();
        }

        // Iterators
        class iterator;
        iterator begin() const;
        iterator end() const;

        // Non-member operators
        friend bool operator==(Path const &lhs, Path const &rhs);
        friend bool operator!=(Path const &lhs, Path const &rhs);
        friend bool operator<(Path const &lhs, Path const &rhs);
        friend bool operator<=(Path const &lhs, Path const &rhs);
        friend bool operator>(Path const &lhs, Path const &rhs);
        friend bool operator>=(Path const &lhs, Path const &rhs);
        friend Path operator/(Path const &lhs, Path const &rhs);

      private:
#ifdef _WIN32
  #pragma warning(push)
  #pragma warning(disable : 4251) // Suppress C4251 for STL members in DLL interface
#endif
        string_type m_path;
#ifdef _WIN32
  #pragma warning(pop)
#endif

        // Helper methods
        void append_separator_if_needed();
        static bool is_separator(value_type);
        size_t find_filename_pos() const;
        size_t find_extension_pos() const;
      };

      /**
       * @brief File status information
       */
      class LUMEX_API FileStatus
      {
      public:
        FileStatus() : m_type(FileType::none), m_perms(Perms::unknown) {}
        explicit FileStatus(FileType type, Perms perms = Perms::unknown) : m_type(type), m_perms(perms) {}

        FileType
        type() const
        {
          return m_type;
        }
        Perms
        permissions() const
        {
          return m_perms;
        }

        void
        type(FileType type)
        {
          m_type = type;
        }
        void
        permissions(Perms perms)
        {
          m_perms = perms;
        }

      private:
        FileType m_type;
        Perms m_perms;
      };

      /**
       * @brief Space information for a filesystem
       */
      class LUMEX_API SpaceInfo
      {
      public:
        std::uintmax_t capacity{};
        std::uintmax_t free{};
        std::uintmax_t available{};
      };

      /**
       * @brief Directory entry representation
       */
      class LUMEX_API DirectoryEntry
      {
      public:
        DirectoryEntry() = default;
        explicit DirectoryEntry(Path const &);

        Path const &
        path() const
        {
          return m_path;
        }
        bool exists() const;
        bool is_regular_file() const;
        bool is_directory() const;
        bool is_symlink() const;
        bool is_block_file() const;
        bool is_character_file() const;
        bool is_fifo() const;
        bool is_socket() const;
        bool is_other() const;

        std::uintmax_t file_size() const;
        FileStatus status() const;
        FileStatus symlink_status() const;

        bool operator==(DirectoryEntry const &rhs) const;
        bool operator!=(DirectoryEntry const &rhs) const;
        bool operator<(DirectoryEntry const &rhs) const;
        bool operator<=(DirectoryEntry const &rhs) const;
        bool operator>(DirectoryEntry const &rhs) const;
        bool operator>=(DirectoryEntry const &rhs) const;

      private:
        Path m_path;
        mutable FileStatus m_status;
        mutable bool m_status_known{};

        void refresh_status() const;
      };

      /**
       * @brief Directory iterator for traversing directory contents
       */
      class LUMEX_API DirectoryIterator
      {
      public:
        // Iterator traits
        using value_type        = DirectoryEntry;
        using difference_type   = std::ptrdiff_t;
        using pointer           = DirectoryEntry const *;
        using reference         = DirectoryEntry const &;
        using iterator_category = std::input_iterator_tag;

        // Constructors
        DirectoryIterator() = default;
        explicit DirectoryIterator(Path const &);
        DirectoryIterator(DirectoryIterator const &);
        DirectoryIterator(DirectoryIterator &&) noexcept;
        ~DirectoryIterator() = default;

        DirectoryIterator &operator=(DirectoryIterator const &);
        DirectoryIterator &operator=(DirectoryIterator &&) noexcept;

        // Iterator operations
        reference operator*() const;
        pointer operator->() const;
        DirectoryIterator &operator++();
        DirectoryIterator operator++(int);

        bool operator==(DirectoryIterator const &) const;
        bool operator!=(DirectoryIterator const &) const;

      private:
        class Impl;
        /*
          Warning C4251 'Lumex::Core::Filesystem::DirectoryIterator::m_impl':
          'std::shared_ptr<Lumex::Core::Filesystem::DirectoryIterator::Impl>'
          needs to have dll-interface to be used by clients of
          'Lumex::Core::Filesystem::DirectoryIterator' appears on MSVC,
          because DirectoryIterator is exported from DLL (LUMEX_API),
          and its private member m_impl is an instance of std::shared_ptr,
          which itself does not have an explicit DLL interface.

          Although std::shared_ptr is a standard library and is usually safely
          used across DLL boundaries (assuming the compiler and settings are
          the same), MSVC issues this warning due to potential ODR (One
          Definition Rule) issues if the std::shared_ptr implementation differs
          between DLL and client code.
        */
#ifdef _WIN32
  #pragma warning(push)
  #pragma warning(disable : 4251)
#endif
        std::shared_ptr<Impl> m_impl;
#ifdef _WIN32
  #pragma warning(pop)
#endif
      };

      /**
       * @brief Main filesystem operations class
       * @details Provides static methods for file system operations
       *          All methods are exception-safe and return result objects
       */
      class LUMEX_API LumexFilesystem
      {
      public:
        // File type tests
        static bool exists(Path const &);
        static bool is_regular_file(Path const &);
        static bool is_directory(Path const &);
        static bool is_symlink(Path const &);
        static bool is_block_file(Path const &);
        static bool is_character_file(Path const &);
        static bool is_fifo(Path const &);
        static bool is_socket(Path const &);
        static bool is_other(Path const &);
        static bool is_empty(Path const &);

        // File operations
        static FilesystemResult<void> copy(Path const &, Path const &);
        static FilesystemResult<void> copy_file(Path const &, Path const &);
        static FilesystemResult<void> copy_symlink(Path const &, Path const &);
        static FilesystemResult<bool> create_directory(Path const &);
        static FilesystemResult<bool> create_directories(Path const &);
        static FilesystemResult<void> create_symlink(Path const &, Path const &);
        static FilesystemResult<void> create_directory_symlink(Path const &, Path const &);
        static FilesystemResult<Path> current_path();
        static FilesystemResult<void> current_path(Path const &);
        static bool equivalent(Path const &, Path const &);
        static FilesystemResult<std::uintmax_t> file_size(Path const &);
        static FilesystemResult<std::time_t> last_write_time(Path const &);
        static FilesystemResult<void> last_write_time(Path const &, std::time_t);
        static FilesystemResult<void> permissions(Path const &, Perms);
        static FilesystemResult<Path> read_symlink(Path const &);
        static FilesystemResult<bool> remove(Path const &);
        static FilesystemResult<std::uintmax_t> remove_all(Path const &);
        static FilesystemResult<void> rename(Path const &, Path const &);
        static FilesystemResult<void> resize_file(Path const &, std::uintmax_t);
        static FilesystemResult<void> move_file(Path const &from, Path const &to_path);
        static FilesystemResult<void> move_directory(Path const &from, Path const &to_path);
        static FilesystemResult<SpaceInfo> space(Path const &);
        static FilesystemResult<FileStatus> status(Path const &);
        static FilesystemResult<FileStatus> symlink_status(Path const &);
        static FilesystemResult<Path> temp_directory_path();

        static std::wstring to_wide_string(std::string const &);
        static std::string from_wide_string(std::wstring const &);

        // Path operations
        static Path absolute(Path const &);
        static Path canonical(Path const &);
        static Path weakly_canonical(Path const &);
        static Path relative(Path const &, Path const &);
        static Path proximate(Path const &, Path const &);

        // Convenience directory iteration
        static std::vector<DirectoryEntry> directory_contents(Path const &);

        // -- Custom own methods (not from standard) --
        /**
         * @brief Retrieve the absolute filesystem path of the running executable.
         *
         * On Windows, uses GetModuleFileNameW;
         * on Unix‑like systems, reads `/proc/self/exe`.
         *
         * @return LumexFilesystem::path Full path to the current executable.
         */
        static Path get_exe_path();

        /**
         * @brief Acquire an exclusive lock on a directory to prevent its deletion.
         *
         * On Windows, opens the directory handle without FILE_SHARE_DELETE;
         * on POSIX, creates a hidden ".lock" file inside the directory and
         * applies a write lock via `fcntl`.
         *
         * @param path Filesystem path to the directory to lock.
         */
        static void lock_directory(Path const &path);

        /**
         * @brief Check if a file is readable by the current process.
         * @param path The filesystem path to check.
         * @return `true` if the file exists, has read permissions, and can be opened for reading; `false` otherwise.
         * @note Logs warnings to `std::cerr` for failures (non-existent file, missing permissions, or open errors).
         */
        static bool is_readable(Path const &path);
        static bool
        is_readable(char const *path)
        {
          return is_readable(Path(path));
        }
        static bool
        is_readable(std::string const &path)
        {
          return is_readable(Path(path));
        }

        /**
         * @brief Check if a file is writable by the current process.
         * @param path The filesystem path to check.
         * @return `true` if the file exists, has write permissions, and can be opened for writing; `false` otherwise.
         * @note Logs warnings to `std::cerr` for failures (non-existent file, missing permissions, or open errors).
         */
        static bool is_writable(Path const &path);
        static bool
        is_writable(char const *path)
        {
          return is_writable(Path(path));
        }
        static bool
        is_writable(std::string const &path)
        {
          return is_writable(Path(path));
        }

        /**
         * @brief Check if a file is both readable and writable by the current process.
         * @param path The filesystem path to check.
         * @return `true` if `isReadable(path) && isWritable(path)`; `false` otherwise.
         */
        static bool is_accessible(Path const &path);
        static bool
        is_accessible(char const *path)
        {
          return is_accessible(Path(path));
        }
        static bool
        is_accessible(std::string const &path)
        {
          return is_accessible(Path(path));
        }

      private:
        // OS-specific implementations
#if LUMEX_OS_WINDOWS
        static FilesystemResult<FileStatus> get_file_status_windows(Path const &, bool);
#else
        static FilesystemResult<FileStatus> get_file_status_posix(Path const &, bool);
#endif
      };

      // Path non-member operators implementation
      inline bool
      operator==(Path const &lhs, Path const &rhs)
      {
        return lhs.m_path == rhs.m_path;
      }

      inline bool
      operator!=(Path const &lhs, Path const &rhs)
      {
        return !(lhs == rhs);
      }

      inline bool
      operator<(Path const &lhs, Path const &rhs)
      {
        return lhs.m_path < rhs.m_path;
      }

      inline bool
      operator<=(Path const &lhs, Path const &rhs)
      {
        return !(rhs < lhs);
      }

      inline bool
      operator>(Path const &lhs, Path const &rhs)
      {
        return rhs < lhs;
      }

      inline bool
      operator>=(Path const &lhs, Path const &rhs)
      {
        return !(lhs < rhs);
      }

      inline Path
      operator/(Path const &lhs, Path const &rhs)
      {
        Path result = lhs;
        result /= rhs;
        return result;
      }

      inline Path
      operator/(Path const &lhs, std::string const &rhs)
      {
        Path result = lhs;
        result /= rhs;
        return result;
      }

      inline Path
      operator/(Path const &lhs, char const *rhs)
      {
        Path result = lhs;
        result /= rhs;
        return result;
      }

      inline Path
      operator/(std::string const &lhs, Path const &rhs)
      {
        return Path(lhs) / rhs;
      }

      inline Path
      operator/(char const *lhs, Path const &rhs)
      {
        return Path(lhs) / rhs;
      }
    } // namespace Filesystem
  } // namespace Core
} // namespace Lumex

// Convenience type aliases
namespace Lumex
{
  using Path                                   = Core::Filesystem::Path;
  using DirectoryEntry                         = Core::Filesystem::DirectoryEntry;
  using DirectoryIterator                      = Core::Filesystem::DirectoryIterator;
  using FileType                               = Core::Filesystem::FileType;
  using FileStatus                             = Core::Filesystem::FileStatus;
  using Perms                                  = Core::Filesystem::Perms;
  using SpaceInfo                              = Core::Filesystem::SpaceInfo;
  using Filesystem                             = Core::Filesystem::LumexFilesystem;

  template <typename T> using FilesystemResult = Core::Filesystem::FilesystemResult<T>;
} // namespace Lumex

inline std::ostream &
operator<<(std::ostream &out, Lumex::Path const &path)
{
  out << path.string();
  return out;
}

inline std::ostream &
operator<<(std::ostream &out, Lumex::DirectoryEntry const &entry)
{
  out << entry.path().string();
  return out;
}

#endif // !LUMEX_FILESYSTEM_HPP
