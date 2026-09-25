var annotated_dup =
[
    [ "lumex", "a00774.html", [
      [ "applied", "a00775.html", [
        [ "hardware", "a00776.html", [
          [ "caps", "a00777.html", [
            [ "cpu_vectorization_info_t", "a00972.html", "a00972" ],
            [ "CPUVectorizationDetector", "a00976.html", "a00976" ],
            [ "hardware_info_t", "a00980.html", "a00980" ],
            [ "HardwareCapabilities", "a00984.html", "a00984" ]
          ] ]
        ] ],
        [ "json", "a00789.html", [
          [ "helper", "a00792.html", [
            [ "LumexJsonHelper", "a00988.html", "a00988" ]
          ] ],
          [ "normalization", "a00794.html", [
            [ "ILumexJsonNormalizer", "a00992.html", "a00992" ],
            [ "LumexJsonSchemaNormalizer", "a00996.html", "a00996" ]
          ] ],
          [ "validation", "a00796.html", [
            [ "ILumexJsonSchemaValidator", "a01000.html", "a01000" ],
            [ "LumexJsonSchemaValidator", "a01004.html", "a01004" ]
          ] ]
        ] ],
        [ "logger", "a00797.html", [
          [ "logger", "a00798.html", [
            [ "log_entry_t", "a01012.html", "a01012" ],
            [ "logger_applied_config_view_t", "a01016.html", "a01016" ],
            [ "logger_config_t", "a01008.html", "a01008" ],
            [ "LumexLogger", "a01020.html", "a01020" ]
          ] ]
        ] ],
        [ "logging", "a00802.html", [
          [ "log", "a00803.html", [
            [ "LumexLogging", "a01024.html", "a01024" ]
          ] ]
        ] ],
        [ "resource_monitor", "a00804.html", [
          [ "monitor", "a00805.html", [
            [ "LumexResourceMonitor", "a01028.html", "a01028" ]
          ] ]
        ] ],
        [ "serial", "a00808.html", [
          [ "enumeration", "a00809.html", [
            [ "port_process_resolver", "a03755.html", "a03755" ],
            [ "serial_port_info_t", "a01032.html", "a01032" ]
          ] ],
          [ "resolver", "a00814.html", [
            [ "port_holder_info_t", "a01044.html", "a01044" ],
            [ "port_holder_resolver", "a01052.html", "a01052" ],
            [ "port_process_resolver", "a01056.html", "a01056" ],
            [ "system_error_formatter", "a01048.html", "a01048" ]
          ] ]
        ] ],
        [ "settings", "a00816.html", [
          [ "factory", "a00817.html", [
            [ "LumexSettingsFactory", "a01060.html", "a01060" ]
          ] ],
          [ "guard", "a00818.html", [
            [ "lumex_settings_key_spec_t", "a01064.html", "a01064" ],
            [ "LumexSettingsGuard", "a01068.html", "a01068" ]
          ] ],
          [ "ini", "a00819.html", [
            [ "LumexSettingsINI", "a01072.html", "a01072" ]
          ] ],
          [ "json", "a00822.html", [
            [ "LumexSettingsJSON", "a01080.html", "a01080" ]
          ] ],
          [ "xml", "a00824.html", [
            [ "LumexSettingsXML", "a01084.html", "a01084" ]
          ] ],
          [ "ILumexSettings", "a01076.html", "a01076" ]
        ] ]
      ] ],
      [ "core", "a00826.html", [
        [ "base64", "a00827.html", [
          [ "decode", "a00832.html", [
            [ "Decoder", "a01088.html", "a01088" ]
          ] ],
          [ "encode", "a00833.html", [
            [ "Encoder", "a01092.html", "a01092" ]
          ] ],
          [ "validate", "a00835.html", [
            [ "Validator", "a01096.html", "a01096" ]
          ] ]
        ] ],
        [ "circular_buffer", "a00836.html", [
          [ "CircularBuffer", "a01100.html", "a01100" ]
        ] ],
        [ "crc", "a00837.html", [
          [ "catalog", "a00838.html", [
            [ "crc_params_t", "a01120.html", "a01120" ]
          ] ],
          [ "parametric", "a00840.html", [
            [ "Detail", "a00841.html", [
              [ "crc_dispatch_t", "a01132.html", null ],
              [ "crc_dispatch_t< Spec, typename std::enable_if<(Spec::kWidth >=kBitsPerByte), void >::type >", "a01140.html", "a01140" ],
              [ "crc_dispatch_t< Spec, typename std::enable_if<(Spec::kWidth< kBitsPerByte), void >::type >", "a01136.html", "a01136" ],
              [ "mask_impl_t", "a01124.html", "a01124" ],
              [ "mask_impl_t< Width, typename std::enable_if< Width==std::numeric_limits< std::uint64_t >::digits, void >::type >", "a01128.html", "a01128" ]
            ] ],
            [ "crc10_atm_spec_t", "a01328.html", "a01328" ],
            [ "crc10_cdma2000_spec_t", "a01168.html", "a01168" ],
            [ "crc10_gsm_spec_t", "a01332.html", "a01332" ],
            [ "crc11_flexray_spec_t", "a01336.html", "a01336" ],
            [ "crc11_umts_spec_t", "a01340.html", "a01340" ],
            [ "crc12_cdma2000_spec_t", "a01172.html", "a01172" ],
            [ "crc12_dect_spec_t", "a01344.html", "a01344" ],
            [ "crc12_gsm_spec_t", "a01348.html", "a01348" ],
            [ "crc12_umts_spec_t", "a01352.html", "a01352" ],
            [ "crc13_bbc_spec_t", "a01356.html", "a01356" ],
            [ "crc14_darc_spec_t", "a01360.html", "a01360" ],
            [ "crc14_gsm_spec_t", "a01364.html", "a01364" ],
            [ "crc15_can_spec_t", "a01368.html", "a01368" ],
            [ "crc15_mpt1327_spec_t", "a01372.html", "a01372" ],
            [ "crc16_arc_spec_t", "a01376.html", "a01376" ],
            [ "crc16_cdma2000_spec_t", "a01176.html", "a01176" ],
            [ "crc16_cms_spec_t", "a01380.html", "a01380" ],
            [ "crc16_dds110_spec_t", "a01384.html", "a01384" ],
            [ "crc16_dect_r_spec_t", "a01388.html", "a01388" ],
            [ "crc16_dect_x_spec_t", "a01392.html", "a01392" ],
            [ "crc16_dnp_spec_t", "a01396.html", "a01396" ],
            [ "crc16_en13757_spec_t", "a01400.html", "a01400" ],
            [ "crc16_genibus_spec_t", "a01404.html", "a01404" ],
            [ "crc16_gsm_spec_t", "a01408.html", "a01408" ],
            [ "crc16_ibm3740_spec_t", "a01184.html", "a01184" ],
            [ "crc16_ibm_sdlc_spec_t", "a01412.html", "a01412" ],
            [ "crc16_iso_iec14443_3_a_spec_t", "a01416.html", "a01416" ],
            [ "crc16_kermit_spec_t", "a01188.html", "a01188" ],
            [ "crc16_lj1200_spec_t", "a01420.html", "a01420" ],
            [ "crc16_m17_spec_t", "a01424.html", "a01424" ],
            [ "crc16_maxim_dow_spec_t", "a01428.html", "a01428" ],
            [ "crc16_mcrf4xx_spec_t", "a01432.html", "a01432" ],
            [ "crc16_modbus_spec_t", "a01192.html", "a01192" ],
            [ "crc16_nrsc5_spec_t", "a01436.html", "a01436" ],
            [ "crc16_opensafety_a_spec_t", "a01440.html", "a01440" ],
            [ "crc16_opensafety_b_spec_t", "a01444.html", "a01444" ],
            [ "crc16_profibus_spec_t", "a01448.html", "a01448" ],
            [ "crc16_riello_spec_t", "a01452.html", "a01452" ],
            [ "crc16_spi_fujitsu_spec_t", "a01456.html", "a01456" ],
            [ "crc16_t10_dif_spec_t", "a01460.html", "a01460" ],
            [ "crc16_teledisk_spec_t", "a01464.html", "a01464" ],
            [ "crc16_tms37157_spec_t", "a01468.html", "a01468" ],
            [ "crc16_umts_spec_t", "a01472.html", "a01472" ],
            [ "crc16_usb_spec_t", "a01476.html", "a01476" ],
            [ "crc16_xmodem_spec_t", "a01480.html", "a01480" ],
            [ "crc17_can_fd_spec_t", "a01484.html", "a01484" ],
            [ "crc21_can_fd_spec_t", "a01488.html", "a01488" ],
            [ "crc24_ble_spec_t", "a01492.html", "a01492" ],
            [ "crc24_flexray_a_spec_t", "a01496.html", "a01496" ],
            [ "crc24_flexray_b_spec_t", "a01500.html", "a01500" ],
            [ "crc24_interlaken_spec_t", "a01504.html", "a01504" ],
            [ "crc24_lte_a_spec_t", "a01508.html", "a01508" ],
            [ "crc24_lte_b_spec_t", "a01512.html", "a01512" ],
            [ "crc24_open_pgp_spec_t", "a01196.html", "a01196" ],
            [ "crc24_os9_spec_t", "a01516.html", "a01516" ],
            [ "crc30_cdma_spec_t", "a01180.html", "a01180" ],
            [ "crc31_philips_spec_t", "a01520.html", "a01520" ],
            [ "crc32_aixm_spec_t", "a01524.html", "a01524" ],
            [ "crc32_autosar_spec_t", "a01528.html", "a01528" ],
            [ "crc32_base91_d_spec_t", "a01532.html", "a01532" ],
            [ "crc32_bzip2_spec_t", "a01536.html", "a01536" ],
            [ "crc32_cd_rom_edc_spec_t", "a01540.html", "a01540" ],
            [ "crc32_cksum_spec_t", "a01544.html", "a01544" ],
            [ "crc32_iscsi_spec_t", "a01204.html", "a01204" ],
            [ "crc32_iso_hdlc_spec_t", "a01200.html", "a01200" ],
            [ "crc32_jamcrc_spec_t", "a01548.html", "a01548" ],
            [ "crc32_mef_spec_t", "a01552.html", "a01552" ],
            [ "crc32_mpeg2_spec_t", "a01556.html", "a01556" ],
            [ "crc32_xfer_spec_t", "a01560.html", "a01560" ],
            [ "crc3_gsm_spec_t", "a01144.html", "a01144" ],
            [ "crc3_rohc_spec_t", "a01212.html", "a01212" ],
            [ "crc40_gsm_spec_t", "a01564.html", "a01564" ],
            [ "crc4_g704_spec_t", "a01216.html", "a01216" ],
            [ "crc4_interlaken_spec_t", "a01220.html", "a01220" ],
            [ "crc5_epc_c1_g2_spec_t", "a01224.html", "a01224" ],
            [ "crc5_g704_spec_t", "a01228.html", "a01228" ],
            [ "crc5_usb_spec_t", "a01148.html", "a01148" ],
            [ "crc64_ecma182_spec_t", "a01208.html", "a01208" ],
            [ "crc64_go_iso_spec_t", "a01568.html", "a01568" ],
            [ "crc64_ms_spec_t", "a01572.html", "a01572" ],
            [ "crc64_nvme_spec_t", "a01576.html", "a01576" ],
            [ "crc64_redis_spec_t", "a01580.html", "a01580" ],
            [ "crc64_we_spec_t", "a01584.html", "a01584" ],
            [ "crc64_xz_spec_t", "a01588.html", "a01588" ],
            [ "crc6_cdma2000_a_spec_t", "a01156.html", "a01156" ],
            [ "crc6_cdma2000_b_spec_t", "a01160.html", "a01160" ],
            [ "crc6_darc_spec_t", "a01232.html", "a01232" ],
            [ "crc6_g704_spec_t", "a01236.html", "a01236" ],
            [ "crc6_gsm_spec_t", "a01240.html", "a01240" ],
            [ "crc7_mmc_spec_t", "a01244.html", "a01244" ],
            [ "crc7_rohc_spec_t", "a01248.html", "a01248" ],
            [ "crc7_umts_spec_t", "a01252.html", "a01252" ],
            [ "crc8_autosar_spec_t", "a01256.html", "a01256" ],
            [ "crc8_bluetooth_spec_t", "a01260.html", "a01260" ],
            [ "crc8_cdma2000_spec_t", "a01164.html", "a01164" ],
            [ "crc8_darc_spec_t", "a01264.html", "a01264" ],
            [ "crc8_dvb_s2_spec_t", "a01268.html", "a01268" ],
            [ "crc8_gsm_a_spec_t", "a01272.html", "a01272" ],
            [ "crc8_gsm_b_spec_t", "a01276.html", "a01276" ],
            [ "crc8_hitag_spec_t", "a01280.html", "a01280" ],
            [ "crc8_i4321_spec_t", "a01284.html", "a01284" ],
            [ "crc8_i_code_spec_t", "a01288.html", "a01288" ],
            [ "crc8_lte_spec_t", "a01292.html", "a01292" ],
            [ "crc8_maxim_dow_spec_t", "a01152.html", "a01152" ],
            [ "crc8_mifare_mad_spec_t", "a01296.html", "a01296" ],
            [ "crc8_nrsc5_spec_t", "a01300.html", "a01300" ],
            [ "crc8_opensafety_spec_t", "a01304.html", "a01304" ],
            [ "crc8_rohc_spec_t", "a01308.html", "a01308" ],
            [ "crc8_sae_j1850_spec_t", "a01312.html", "a01312" ],
            [ "crc8_smbus_spec_t", "a01316.html", "a01316" ],
            [ "crc8_tech3250_spec_t", "a01320.html", "a01320" ],
            [ "crc8_wcdma_spec_t", "a01324.html", "a01324" ],
            [ "CrcParametric", "a01592.html", "a01592" ]
          ] ]
        ] ],
        [ "environment", "a00842.html", [
          [ "env", "a00843.html", [
            [ "LumexEnvironment", "a01596.html", "a01596" ]
          ] ]
        ] ],
        [ "exceptions", "a00844.html", [
          [ "crash", "a00845.html", [
            [ "LumexCrashHandler", "a01612.html", "a01612" ]
          ] ],
          [ "exception", "a00846.html", [
            [ "LumexBaseException", "a01616.html", "a01616" ]
          ] ],
          [ "stacktrace", "a00850.html", [
            [ "hash", "a01624.html", null ],
            [ "hash< LumexBasicStacktrace< Allocator > >", "a01628.html", "a01628" ],
            [ "LumexBasicStacktrace", "a01620.html", "a01620" ],
            [ "LumexStacktraceEntry", "a01632.html", "a01632" ]
          ] ]
        ] ],
        [ "expected", "a00852.html", [
          [ "error", "a00853.html", [
            [ "BadExpectedAccess", "a01640.html", "a01640" ],
            [ "Unexpected", "a01644.html", "a01644" ]
          ] ],
          [ "result", "a00854.html", [
            [ "BadExpectedAccess", "a03767.html", "a03767" ],
            [ "Expected", "a01648.html", "a01648" ],
            [ "Expected< void, ErrorType >", "a01668.html", "a01668" ],
            [ "in_place_tag", "a01656.html", null ],
            [ "unexpect_t", "a01664.html", null ],
            [ "Unexpected", "a03771.html", "a03771" ],
            [ "Unit", "a01660.html", null ]
          ] ]
        ] ],
        [ "filesystem", "a00855.html", [
          [ "fs", "a00856.html", [
            [ "lumex", null, [
              [ "directory_iterator", null, [
                [ "Impl", "a01676.html", "a01676" ]
              ] ]
            ] ],
            [ "directory_entry", "a01700.html", "a01700" ],
            [ "directory_iterator", "a01704.html", "a01704" ],
            [ "file_status", "a01692.html", "a01692" ],
            [ "filesystem_result", "a01680.html", "a01680" ],
            [ "filesystem_result< void >", "a01684.html", "a01684" ],
            [ "lumex_filesystem", "a01708.html", "a01708" ],
            [ "path", "a01688.html", "a01688" ],
            [ "space_info", "a01696.html", "a01696" ]
          ] ]
        ] ],
        [ "fmt", "a00859.html", [
          [ "Detail", "a00860.html", [
            [ "all_formattable", "a01912.html", null ],
            [ "all_formattable< Char >", "a01916.html", null ],
            [ "all_formattable< Char, First, Rest... >", "a01920.html", null ],
            [ "arg_ref_t", "a01748.html", "a01748" ],
            [ "BasicStringRef", "a01720.html", "a01720" ],
            [ "Buffer", "a01724.html", "a01724" ],
            [ "BuiltinFormatter", "a01836.html", "a01836" ],
            [ "ChronoFormatter", "a01884.html", "a01884" ],
            [ "civil_time_t", "a01880.html", "a01880" ],
            [ "count_named", "a01776.html", null ],
            [ "count_named< First, Rest... >", "a01784.html", null ],
            [ "count_named<>", "a01780.html", null ],
            [ "CountingBuffer", "a01736.html", "a01736" ],
            [ "custom_arg_t", "a01792.html", "a01792" ],
            [ "ElementFormatter", "a01932.html", "a01932" ],
            [ "float_limits_t", "a01820.html", null ],
            [ "float_limits_t< double >", "a01828.html", "a01828" ],
            [ "float_limits_t< float >", "a01824.html", "a01824" ],
            [ "float_limits_t< long double >", "a01832.html", "a01832" ],
            [ "format_arg_store_t", "a01808.html", "a01808" ],
            [ "format_arg_t", "a01796.html", "a01796" ],
            [ "format_specs_t", "a01752.html", "a01752" ],
            [ "FormatHandler", "a01840.html", "a01840" ],
            [ "is_formattable_range", "a01904.html", null ],
            [ "is_formattable_range< Range, Char, typename std::enable_if< lumex::core::utility::traits::range::is_iterable< Range >::value >::type >", "a01908.html", null ],
            [ "is_named_arg", "a01760.html", null ],
            [ "is_named_arg< named_arg_t< Char, T > >", "a01764.html", null ],
            [ "IteratorBuffer", "a01732.html", "a01732" ],
            [ "named_arg_entry_t", "a01804.html", "a01804" ],
            [ "named_arg_t", "a01756.html", "a01756" ],
            [ "range_element", "a01896.html", "a01896" ],
            [ "range_element< Range, Char, typename std::enable_if< !has_formatter< Char, typename std::decay< typename lumex::core::utility::traits::range::range_reference< Range >::type >::type >() &&has_formatter< Char, typename Range::value_type >() &&std::is_convertible< typename lumex::core::utility::traits::range::range_reference< Range >::type, typename Range::value_type >::value >::type >", "a01900.html", "a01900" ],
            [ "string_arg_t", "a01788.html", "a01788" ],
            [ "StringBuffer", "a01728.html", "a01728" ],
            [ "TruncatingBuffer", "a01740.html", "a01740" ],
            [ "tuple_each", "a01924.html", "a01924" ],
            [ "tuple_each< Count, Count >", "a01928.html", "a01928" ],
            [ "TupleFormatter", "a01936.html", "a01936" ],
            [ "unwrap_named", "a01768.html", "a01768" ],
            [ "unwrap_named< named_arg_t< Char, T > >", "a01772.html", "a01772" ]
          ] ],
          [ "BasicAppender", "a01744.html", "a01744" ],
          [ "BasicFormatArgs", "a01812.html", "a01812" ],
          [ "BasicFormatContext", "a01816.html", "a01816" ],
          [ "BasicFormatParseContext", "a01712.html", "a01712" ],
          [ "BasicFormatString", "a01848.html", "a01848" ],
          [ "format_to_n_result_t", "a01872.html", "a01872" ],
          [ "Formatter", "a01716.html", "a01716" ],
          [ "Formatter< area_t >", "a02544.html", "a02544" ],
          [ "Formatter< date_t >", "a02508.html", "a02508" ],
          [ "Formatter< Enum, Char, typename std::enable_if< lumex::core::utility::traits::enums::is_reflected_enum< Enum >::value >::type >", "a01856.html", "a01856" ],
          [ "Formatter< pipe_list_t >", "a02528.html", "a02528" ],
          [ "Formatter< point_t >", "a02512.html", "a02512" ],
          [ "Formatter< pressure_t >", "a02500.html", "a02500" ],
          [ "Formatter< pressure_t, wchar_t >", "a02504.html", "a02504" ],
          [ "Formatter< Range, Char, typename std::enable_if< Detail::is_formattable_range< Range, Char >::value >::type >", "a01948.html", "a01948" ],
          [ "Formatter< register_dump_t >", "a02532.html", "a02532" ],
          [ "Formatter< stars_t, Char >", "a02516.html", "a02516" ],
          [ "Formatter< std::chrono::duration< Rep, Period >, Char, typename std::enable_if< Detail::has_formatter< Char, Rep >() &&std::is_arithmetic< Rep >::value >::type >", "a01888.html", "a01888" ],
          [ "Formatter< std::chrono::time_point< std::chrono::system_clock, Duration >, Char, typename std::enable_if<!std::chrono::treat_as_floating_point< typename Duration::rep >::value >::type >", "a01892.html", "a01892" ],
          [ "Formatter< std::pair< First, Second >, Char, typename std::enable_if< Detail::all_formattable< Char, First, Second >::value >::type >", "a01940.html", "a01940" ],
          [ "Formatter< std::tuple< Types... >, Char, typename std::enable_if< Detail::all_formattable< Char, Types... >::value >::type >", "a01944.html", "a01944" ],
          [ "Formatter< streamed_t< T >, Char, void >", "a01868.html", "a01868" ],
          [ "Formatter< T, Char, typename std::enable_if< Detail::builtin_kind< Char, typename std::decay< T >::type >() !=Detail::ArgKind::none >::type >", "a01852.html", "a01852" ],
          [ "OstreamFormatter", "a01860.html", "a01860" ],
          [ "runtime_format_string_t", "a01844.html", "a01844" ],
          [ "streamed_t", "a01864.html", "a01864" ],
          [ "try_format_result_t", "a01876.html", "a01876" ]
        ] ],
        [ "generators", "a00861.html", [
          [ "number_generator", "a00862.html", [
            [ "NumberGenerator", "a01952.html", "a01952" ]
          ] ]
        ] ],
        [ "math", "a00863.html", [
          [ "ops", "a00864.html", [
            [ "Detail", "a00866.html", [
              [ "difference_type", "a01996.html", null ],
              [ "difference_type< T, U, true >", "a02000.html", "a02000" ],
              [ "floating_distance", "a02012.html", null ],
              [ "floating_distance< T, U, true >", "a02016.html", "a02016" ],
              [ "integral_distance", "a02004.html", null ],
              [ "integral_distance< T, U, true >", "a02008.html", "a02008" ],
              [ "is_numeric_range", "a01968.html", null ],
              [ "is_numeric_range< Range, typename voider< decltype(adl_begin(std::declval< typename range_object< Range >::type & >()) !=adl_end(std::declval< typename range_object< Range >::type & >())), decltype(++std::declval< decltype(adl_begin(std::declval< typename range_object< Range >::type & >())) & >()), decltype(*adl_begin(std::declval< typename range_object< Range >::type & >()))>::type >", "a01972.html", null ],
              [ "numeric_common", "a01988.html", null ],
              [ "numeric_common< T, U, true >", "a01992.html", "a01992" ],
              [ "numeric_range_value", "a01976.html", null ],
              [ "numeric_range_value< Range, true >", "a01980.html", "a01980" ],
              [ "range_object", "a01964.html", "a01964" ],
              [ "range_pair_result", "a02020.html", null ],
              [ "range_pair_result< Range1, Range2, true >", "a02024.html", "a02024" ],
              [ "range_scalar_result", "a02028.html", null ],
              [ "range_scalar_result< Range, Scalar, true >", "a02032.html", "a02032" ],
              [ "sqrt_result", "a01984.html", "a01984" ],
              [ "voider", "a01960.html", "a01960" ]
            ] ],
            [ "traits", "a00865.html", [
              [ "is_numeric", "a01956.html", null ]
            ] ]
          ] ]
        ] ],
        [ "optional", "a00867.html", [
          [ "opt", "a00868.html", [
            [ "in_place_t", "a02044.html", "a02044" ],
            [ "LumexBadOptionalAccess", "a02052.html", "a02052" ],
            [ "nullopt_t", "a02036.html", "a02036" ],
            [ "optional", "a02056.html", "a02056" ]
          ] ]
        ] ],
        [ "reflection", "a00869.html", [
          [ "field_reflection", "a00870.html", [
            [ "detail", "a00871.html", [
              [ "aggregate_traits", "a02100.html", "a02100" ],
              [ "any_field", "a02080.html", "a02080" ],
              [ "can_construct_n", "a02084.html", "a02084" ],
              [ "count_fields_impl", "a02088.html", null ],
              [ "count_fields_impl< Aggregate, Lo, Hi, false >", "a02096.html", "a02096" ],
              [ "count_fields_impl< Aggregate, Lo, Hi, true >", "a02092.html", "a02092" ],
              [ "index_sequence", "a02064.html", null ],
              [ "make_index_sequence", "a02076.html", null ],
              [ "make_index_sequence_impl", "a02068.html", null ],
              [ "make_index_sequence_impl< 0, I... >", "a02072.html", "a02072" ]
            ] ],
            [ "tuple_size", "a02104.html", "a02104" ]
          ] ]
        ] ],
        [ "string_view", "a00878.html", [
          [ "view", "a00879.html", [
            [ "LumexStringView", "a02108.html", "a02108" ],
            [ "LumexWStringView", "a02112.html", "a02112" ]
          ] ]
        ] ],
        [ "temporary", "a00880.html", [
          [ "tmp", "a00881.html", [
            [ "LumexTemporary", "a02124.html", "a02124" ],
            [ "TemporaryDirectory", "a02116.html", "a02116" ],
            [ "TemporaryFile", "a02120.html", "a02120" ]
          ] ]
        ] ],
        [ "time", "a00882.html", [
          [ "clock", "a00883.html", [
            [ "LumexTime", "a02128.html", "a02128" ]
          ] ],
          [ "timer", "a00885.html", [
            [ "LumexTimer", "a02132.html", "a02132" ]
          ] ]
        ] ],
        [ "utility", "a00886.html", [
          [ "callback", "a00888.html", [
            [ "LumexCallbackSlot", "a02136.html", null ],
            [ "LumexCallbackSlot< Tag, Result(Args...)>", "a02140.html", "a02140" ]
          ] ],
          [ "cast", "a00889.html", [
            [ "BadDownCast", "a02148.html", "a02148" ]
          ] ],
          [ "dump", "a00894.html", [
            [ "CoreDumpGenerator", "a02160.html", "a02160" ],
            [ "DumpConfiguration", "a02152.html", "a02152" ],
            [ "DumpFactory", "a02156.html", "a02156" ]
          ] ],
          [ "numeric", "a00902.html", [
            [ "comparison_traits", "a02172.html", "a02172" ],
            [ "safe_compare_impl_helper", "a02176.html", null ],
            [ "safe_compare_impl_helper< T, T, typename std::enable_if< std::is_floating_point< traits::meta::CleanType< T > >::value >::type >", "a02184.html", "a02184" ],
            [ "safe_compare_impl_helper< T, T, typename std::enable_if< std::is_integral< traits::meta::CleanType< T > >::value >::type >", "a02180.html", "a02180" ],
            [ "safe_compare_impl_helper< T, U, typename std::enable_if< std::is_floating_point< traits::meta::CleanType< T > >::value &&std::is_floating_point< traits::meta::CleanType< U > >::value &&!std::is_same< traits::meta::CleanType< T >, traits::meta::CleanType< U > >::value >::type >", "a02192.html", "a02192" ],
            [ "safe_compare_impl_helper< T, U, typename std::enable_if< std::is_integral< traits::meta::CleanType< T > >::value &&std::is_integral< traits::meta::CleanType< U > >::value &&!std::is_same< traits::meta::CleanType< T >, traits::meta::CleanType< U > >::value >::type >", "a02188.html", "a02188" ],
            [ "safe_compare_impl_helper< T, U, typename std::enable_if<(std::is_integral< traits::meta::CleanType< T > >::value &&std::is_floating_point< traits::meta::CleanType< U > >::value)||(std::is_floating_point< traits::meta::CleanType< T > >::value &&std::is_integral< traits::meta::CleanType< U > >::value)>::type >", "a02196.html", "a02196" ],
            [ "SafeComparator", "a02200.html", "a02200" ]
          ] ],
          [ "traits", "a00907.html", [
            [ "enums", "a00917.html", [
              [ "is_reflected_enum", "a02476.html", null ],
              [ "is_reflected_enum< T, meta::void_t< decltype(toString(std::declval< T >()))> >", "a02480.html", null ]
            ] ],
            [ "invoke", "a00909.html", [
              [ "detail", "a00910.html", [
                [ "invoke_impl", "a02252.html", "a02252" ],
                [ "invoke_impl< MT B::* >", "a02256.html", "a02256" ],
                [ "invoke_result_impl", "a02260.html", null ],
                [ "invoke_result_impl< meta::void_t< decltype(INVOKE(std::declval< F >(), std::declval< Args >()...))>, F, Args... >", "a02264.html", "a02264" ],
                [ "is_reference_wrapper", "a02244.html", null ],
                [ "is_reference_wrapper< std::reference_wrapper< U > >", "a02248.html", null ]
              ] ],
              [ "invoke_result", "a02268.html", null ],
              [ "is_callable", "a02292.html", null ],
              [ "is_callable_signature", "a02284.html", null ],
              [ "is_callable_signature< Func(Args...)>", "a02288.html", null ],
              [ "is_invocable", "a02280.html", null ],
              [ "result_of", "a02272.html", null ],
              [ "result_of< Func(Args...)>", "a02276.html", null ]
            ] ],
            [ "meta", "a00908.html", [
              [ "default_return", "a02220.html", "a02220" ],
              [ "default_return< T * >", "a02228.html", "a02228" ],
              [ "default_return< void >", "a02224.html", "a02224" ],
              [ "has_type", "a02208.html", null ],
              [ "has_type< T, void_t< typename T::type > >", "a02212.html", null ],
              [ "indirection_of", "a02232.html", "a02232" ],
              [ "indirection_of< T & >", "a02240.html", "a02240" ],
              [ "indirection_of< T * >", "a02236.html", "a02236" ],
              [ "make_void", "a02204.html", "a02204" ],
              [ "type_identity", "a02216.html", "a02216" ]
            ] ],
            [ "numeric", "a00918.html", [
              [ "is_safe_comparable", "a02484.html", "a02484" ]
            ] ],
            [ "range", "a00913.html", [
              [ "has_convertible_indexed_access", "a02404.html", null ],
              [ "has_convertible_indexed_access< T, To, meta::void_t< decltype(std::declval< T >()[std::declval< std::size_t >()])> >", "a02408.html", null ],
              [ "has_convertible_size", "a02396.html", null ],
              [ "has_convertible_size< T, meta::void_t< decltype(std::declval< T >().size())> >", "a02400.html", null ],
              [ "has_elements_convertible_to", "a02372.html", null ],
              [ "has_elements_convertible_to< Range, To, meta::void_t< typename range_reference< Range >::type > >", "a02376.html", null ],
              [ "has_key_type", "a02380.html", null ],
              [ "has_key_type< T, meta::void_t< typename T::key_type > >", "a02384.html", null ],
              [ "has_mapped_type", "a02388.html", null ],
              [ "has_mapped_type< T, meta::void_t< typename T::mapped_type > >", "a02392.html", null ],
              [ "has_streamable_elements", "a02364.html", null ],
              [ "has_streamable_elements< Range, meta::void_t< typename range_reference< Range >::type > >", "a02368.html", null ],
              [ "is_iterable", "a02356.html", null ],
              [ "is_iterable< Range, meta::void_t< typename range_reference< Range >::type > >", "a02360.html", null ],
              [ "range_reference", "a02348.html", null ],
              [ "range_reference< Range, meta::void_t< decltype(std::begin(std::declval< Range const & >()) !=std::end(std::declval< Range const & >())), decltype(++std::declval< decltype(std::begin(std::declval< Range const & >())) & >()), decltype(*std::begin(std::declval< Range const & >()))> >", "a02352.html", "a02352" ]
            ] ],
            [ "stream", "a00911.html", [
              [ "detail", "a00912.html", [
                [ "is_address_streamed", "a02304.html", null ],
                [ "is_address_streamed< std::shared_ptr< T > >", "a02312.html", null ],
                [ "is_address_streamed< std::unique_ptr< T, D > >", "a02308.html", null ],
                [ "is_streamable_expression", "a02296.html", null ],
                [ "is_streamable_expression< T, meta::void_t< decltype(std::declval< std::ostream & >()<< std::declval< T >())> >", "a02300.html", null ]
              ] ],
              [ "all_ostreamable", "a02320.html", null ],
              [ "all_ostreamable< First, Rest... >", "a02328.html", null ],
              [ "all_ostreamable<>", "a02324.html", null ],
              [ "all_streamable", "a02336.html", null ],
              [ "all_streamable< First, Rest... >", "a02344.html", null ],
              [ "all_streamable<>", "a02340.html", null ],
              [ "is_ostreamable", "a02316.html", null ],
              [ "is_streamable", "a02332.html", null ]
            ] ],
            [ "string", "a00914.html", [
              [ "is_any_string", "a02420.html", null ],
              [ "is_any_string< T, meta::void_t< typename T::value_type > >", "a02424.html", null ],
              [ "is_string_like", "a02412.html", null ],
              [ "is_string_like< T, Char, meta::void_t< decltype(std::declval< T const & >().data()), decltype(std::declval< T const & >().size()), decltype(T::npos), typename T::value_type > >", "a02416.html", null ]
            ] ],
            [ "tuple", "a00915.html", [
              [ "is_pair_like", "a02428.html", null ],
              [ "is_pair_like< std::pair< First, Second > >", "a02432.html", null ],
              [ "is_pair_like< std::tuple< First, Second > >", "a02436.html", null ]
            ] ],
            [ "value", "a00916.html", [
              [ "is_expected", "a02460.html", null ],
              [ "is_expected< lumex::core::expected::result::Expected< S, E > >", "a02464.html", null ],
              [ "is_optional", "a02440.html", null ],
              [ "is_optional< lumex::core::optional::opt::optional< T > >", "a02456.html", null ],
              [ "is_optional< T const >", "a02444.html", null ],
              [ "is_optional< T const volatile >", "a02452.html", null ],
              [ "is_optional< T volatile >", "a02448.html", null ],
              [ "is_optional_like", "a02468.html", null ],
              [ "is_optional_like< T, meta::void_t< decltype(std::declval< T const & >().has_value()), decltype(*std::declval< T const & >())> >", "a02472.html", null ]
            ] ]
          ] ]
        ] ]
      ] ],
      [ "examples", "a00934.html", [
        [ "logger", "a00935.html", [
          [ "temporary_config_file_t", "a02552.html", "a02552" ]
        ] ]
      ] ],
      [ "xml", "a00937.html", [
        [ "attribute", "a00938.html", [
          [ "XmlAttribute", "a02556.html", "a02556" ],
          [ "XmlAttributeBase", "a02560.html", "a02560" ],
          [ "XmlAttributeIterator", "a02596.html", "a02596" ]
        ] ],
        [ "document", "a00944.html", [
          [ "XmlDocument", "a02568.html", "a02568" ],
          [ "XmlDocumentBase", "a02572.html", "a02572" ]
        ] ],
        [ "memory", "a00945.html", [
          [ "XmlAllocator", "a02576.html", "a02576" ],
          [ "XmlMemoryPage", "a02580.html", "a02580" ]
        ] ],
        [ "node", "a00940.html", [
          [ "name_null_sentry", "a02604.html", "a02604" ],
          [ "XmlNamedNodeIterator", "a02592.html", "a02592" ],
          [ "XmlNode", "a02584.html", "a02584" ],
          [ "XmlNodeBase", "a02600.html", "a02600" ],
          [ "XmlNodeIterator", "a02588.html", "a02588" ]
        ] ],
        [ "range", "a00952.html", [
          [ "XmlObjectRange", "a02608.html", "a02608" ]
        ] ],
        [ "text", "a00950.html", [
          [ "xml_parse_result_t", "a02628.html", "a02628" ],
          [ "XmlParser", "a02624.html", "a02624" ],
          [ "XmlText", "a02632.html", "a02632" ]
        ] ],
        [ "tree", "a00951.html", [
          [ "XmlTreeWalker", "a02636.html", "a02636" ]
        ] ],
        [ "types", "a00953.html", [
          [ "Types", "a00954.html", [
            [ "xml_extra_buffer", "a02644.html", "a02644" ],
            [ "xml_mem_str_header_t", "a02640.html", "a02640" ]
          ] ]
        ] ],
        [ "utility", "a00939.html", [
          [ "latin1_decoder", "a02672.html", "a02672" ],
          [ "latin1_writer", "a02696.html", "a02696" ],
          [ "opt_false", "a02652.html", null ],
          [ "opt_true", "a02656.html", null ],
          [ "utf16_counter", "a02660.html", "a02660" ],
          [ "utf16_decoder", "a02664.html", "a02664" ],
          [ "utf16_writer", "a02684.html", "a02684" ],
          [ "utf32_counter", "a02688.html", "a02688" ],
          [ "utf32_decoder", "a02668.html", "a02668" ],
          [ "utf32_writer", "a02692.html", "a02692" ],
          [ "utf8_counter", "a02676.html", "a02676" ],
          [ "utf8_decoder", "a02716.html", "a02716" ],
          [ "utf8_writer", "a02680.html", "a02680" ],
          [ "wchar_decoder", "a02712.html", "a02712" ],
          [ "wchar_selector", "a02700.html", null ],
          [ "wchar_selector< 2 >", "a02704.html", "a02704" ],
          [ "wchar_selector< 4 >", "a02708.html", "a02708" ],
          [ "XmlCleaner", "a02648.html", "a02648" ]
        ] ],
        [ "writer", "a00955.html", [
          [ "IXmlWriter", "a02720.html", "a02720" ],
          [ "XmlBufferedWriter", "a02724.html", "a02724" ],
          [ "XmlWriterFile", "a02732.html", "a02732" ],
          [ "XmlWriterStream", "a02736.html", "a02736" ]
        ] ],
        [ "xpath", "a00946.html", [
          [ "ast", "a00957.html", [
            [ "axis_to_type", "a02760.html", "a02760" ],
            [ "XPathAstNode", "a02764.html", "a02764" ]
          ] ],
          [ "context", "a00960.html", [
            [ "XPathContext", "a02772.html", "a02772" ]
          ] ],
          [ "document", "a00961.html", [
            [ "document_order_comparator", "a02776.html", "a02776" ]
          ] ],
          [ "exception", "a00962.html", [
            [ "XPathException", "a02780.html", "a02780" ]
          ] ],
          [ "memory", "a00963.html", [
            [ "XPathAllocator", "a02784.html", "a02784" ],
            [ "XPathAllocatorCapture", "a02788.html", "a02788" ],
            [ "XPathMemoryBlock", "a02792.html", "a02792" ],
            [ "XPathStack", "a02800.html", "a02800" ],
            [ "XPathStackData", "a02804.html", "a02804" ]
          ] ],
          [ "node", "a00947.html", [
            [ "XPathNode", "a02808.html", "a02808" ],
            [ "XPathNodeSet", "a02812.html", "a02812" ],
            [ "XPathNodeSetRaw", "a02816.html", "a02816" ]
          ] ],
          [ "parser", "a00964.html", [
            [ "lexer", "a00965.html", [
              [ "XPathLexer", "a02820.html", "a02820" ],
              [ "XPathLexerString", "a02824.html", "a02824" ]
            ] ],
            [ "xpath_parse_result_t", "a02836.html", "a02836" ],
            [ "XPathParser", "a02832.html", "a02832" ]
          ] ],
          [ "query", "a00949.html", [
            [ "XPathQuery", "a02840.html", "a02840" ]
          ] ],
          [ "string", "a00967.html", [
            [ "XPathString", "a02848.html", "a02848" ]
          ] ],
          [ "variable", "a00948.html", [
            [ "xpath_variable_boolean", "a02856.html", "a02856" ],
            [ "xpath_variable_node_set", "a02868.html", "a02868" ],
            [ "xpath_variable_number", "a02860.html", "a02860" ],
            [ "xpath_variable_string", "a02864.html", "a02864" ],
            [ "XPathVariable", "a02852.html", "a02852" ],
            [ "XPathVariableSet", "a02872.html", "a02872" ]
          ] ],
          [ "XPathQueryImpl", "a02844.html", "a02844" ]
        ] ]
      ] ]
    ] ],
    [ "std", "a00800.html", [
      [ "hash< lumex::core::exceptions::stacktrace::LumexStacktraceEntry >", "a01636.html", "a01636" ],
      [ "hash<::lumex::core::optional::opt::optional< T > >", "a02060.html", "a02060" ]
    ] ],
    [ "BadExpectedAccess", "a03759.html", "a03759" ],
    [ "binary_op_t", "a02828.html", "a02828" ],
    [ "Expected", "a03775.html", "a03775" ],
    [ "FormatError", "a02893.html", null ],
    [ "gap", "a02612.html", "a02612" ],
    [ "hardware_info_t", "a03747.html", "a03747" ],
    [ "HardwareCapabilities", "a03751.html", "a03751" ],
    [ "in_place_tag", "a03783.html", null ],
    [ "Logger", "a02889.html", null ],
    [ "lumex_enum_traits_t", "a02488.html", null ],
    [ "LumexJsonHelper", "a03795.html", "a03795" ],
    [ "LumexJsonSchemaException", "a02881.html", null ],
    [ "LumexJsonSchemaNormalizer", "a03799.html", "a03799" ],
    [ "LumexJsonSchemaTraverser", "a02885.html", null ],
    [ "LumexJsonSchemaValidator", "a03803.html", "a03803" ],
    [ "LumexResourceMonitor", "a03811.html", "a03811" ],
    [ "LumexStringView", "a03807.html", "a03807" ],
    [ "SafeComparator", "a03791.html", "a03791" ],
    [ "strconv_attribute_impl", "a02616.html", "a02616" ],
    [ "strconv_pcdata_impl", "a02620.html", "a02620" ],
    [ "unexpect_t", "a03787.html", null ],
    [ "Unexpected", "a03763.html", "a03763" ],
    [ "xml_stream_chunk", "a02564.html", "a02564" ]
];