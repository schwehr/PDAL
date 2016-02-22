/******************************************************************************
* Copyright (c) 2016, Bradley J Chambers (brad.chambers@gmail.com)
*
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following
* conditions are met:
*
*     * Redistributions of source code must retain the above copyright
*       notice, this list of conditions and the following disclaimer.
*     * Redistributions in binary form must reproduce the above copyright
*       notice, this list of conditions and the following disclaimer in
*       the documentation and/or other materials provided
*       with the distribution.
*     * Neither the name of Hobu, Inc. or Flaxen Geo Consulting nor the
*       names of its contributors may be used to endorse or promote
*       products derived from this software without specific prior
*       written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
* FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
* COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
* INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
* BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
* OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
* AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
* OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
* OF SUCH DAMAGE.
****************************************************************************/

#include <pdal/pdal_test_main.hpp>

#include "Support.hpp"

#include <pdal/PipelineManager.hpp>
#include <pdal/util/FileUtils.hpp>
#include <LasReader.hpp>
#include <LasWriter.hpp>
#include <DecimationFilter.hpp>
#include <MergeFilter.hpp>

using namespace pdal;

TEST(AdditionalMergeTest, merge_filter_and_reader_with_manager)
{
    const char * outfile = "merge_filter_and_reader_with_manager.las";
    FileUtils::deleteFile(outfile);

    PipelineManager mgr;

    Options optsR;
    optsR.add("filename", Support::datapath("las/1.2-with-color.las"));
    Stage& reader1 = mgr.addReader("readers.las");
    reader1.setOptions(optsR);
    Stage& reader2 = mgr.addReader("readers.las");
    reader2.setOptions(optsR);

    Options optsF;
    optsF.add("step", 3);
    optsF.add("offset", 1);
    Stage& filter = mgr.addFilter("filters.decimation");
    filter.setInput(reader1);
    filter.setOptions(optsF);

    Stage& merge = mgr.addFilter("filters.merge");
    merge.setInput(filter);
    merge.setInput(reader2);

    Options optsW;
    optsW.add("filename", outfile, "file to write to");
    Stage& writer = mgr.addWriter("writers.las");
    writer.setInput(merge);
    writer.setOptions(optsW);

    std::cout << "E" << std::endl;
    point_count_t np = mgr.execute();
    std::cout << "Done" << std::endl;
    EXPECT_EQ(1420U, np);

    EXPECT_TRUE(!std::ifstream(outfile).fail());
}

TEST(AdditionalMergeTest, merge_reader_and_filter_with_manager)
{
    const char * outfile = "merge_reader_and_filter_with_manager.las";
    FileUtils::deleteFile(outfile);

    PipelineManager mgr;

    Options optsR;
    optsR.add("filename", Support::datapath("las/1.2-with-color.las"));
    Stage& reader1 = mgr.addReader("readers.las");
    reader1.setOptions(optsR);
    Stage& reader2 = mgr.addReader("readers.las");
    reader2.setOptions(optsR);

    Options optsF;
    optsF.add("step", 3);
    optsF.add("offset", 1);
    Stage& filter = mgr.addFilter("filters.decimation");
    filter.setInput(reader1);
    filter.setOptions(optsF);

    Stage& merge = mgr.addFilter("filters.merge");
    merge.setInput(reader2);
    merge.setInput(filter);

    Options optsW;
    optsW.add("filename", outfile, "file to write to");
    Stage& writer = mgr.addWriter("writers.las");
    writer.setInput(merge);
    writer.setOptions(optsW);

    std::cout << "E" << std::endl;
    point_count_t np = mgr.execute();
    std::cout << "Done" << std::endl;
    EXPECT_EQ(1420U, np);

    EXPECT_TRUE(!std::ifstream(outfile).fail());
}

TEST(AdditionalMergeTest, merge_filter_and_reader_without_manager)
{
    const char * outfile = "merge_filter_and_reader_without_manager.las";
    FileUtils::deleteFile(outfile);

    Options optsR;
    optsR.add("filename", Support::datapath("las/1.2-with-color.las"));
    LasReader reader1;
    reader1.setOptions(optsR);
    LasReader reader2;
    reader2.setOptions(optsR);

    Options optsF;
    optsF.add("step", 3);
    optsF.add("offset", 1);
    DecimationFilter filter;
    filter.setInput(reader1);
    filter.setOptions(optsF);

    MergeFilter merge;
    merge.setInput(filter);
    merge.setInput(reader2);

    Options optsW;
    optsW.add("filename", outfile, "file to write to");
    LasWriter writer;
    writer.setInput(merge);
    writer.setOptions(optsW);

    PointTable table;
    writer.prepare(table);

    std::cout << "E" << std::endl;
    PointViewSet vs = writer.execute(table);
    EXPECT_EQ(1u, vs.size());
    point_count_t np = (*vs.begin())->size();
    std::cout << "Done" << std::endl;
    EXPECT_EQ(1420U, np);

    EXPECT_TRUE(!std::ifstream(outfile).fail());
}

TEST(AdditionalMergeTest, merge_reader_and_filter_without_manager)
{
    const char * outfile = "merge_reader_and_filter_without_manager.las";
    FileUtils::deleteFile(outfile);

    Options optsR;
    optsR.add("filename", Support::datapath("las/1.2-with-color.las"));
    LasReader reader1;
    reader1.setOptions(optsR);
    LasReader reader2;
    reader2.setOptions(optsR);

    Options optsF;
    optsF.add("step", 3);
    optsF.add("offset", 1);
    DecimationFilter filter;
    filter.setInput(reader1);
    filter.setOptions(optsF);

    MergeFilter merge;
    merge.setInput(reader2);
    merge.setInput(filter);

    Options optsW;
    optsW.add("filename", outfile, "file to write to");
    LasWriter writer;
    writer.setInput(merge);
    writer.setOptions(optsW);

    PointTable table;
    writer.prepare(table);

    std::cout << "E" << std::endl;
    PointViewSet vs = writer.execute(table);
    EXPECT_EQ(1u, vs.size());
    point_count_t np = (*vs.begin())->size();
    std::cout << "Done" << std::endl;
    EXPECT_EQ(1420U, np);

    EXPECT_TRUE(!std::ifstream(outfile).fail());
}

TEST(AdditionalMergeTest, filter_and_reader_writer_inputs_with_manager)
{
    const char * outfile = "filter_and_reader_writer_inputs_with_manager.las";
    FileUtils::deleteFile(outfile);

    PipelineManager mgr;

    Options optsR;
    optsR.add("filename", Support::datapath("las/1.2-with-color.las"));
    Stage& reader1 = mgr.addReader("readers.las");
    reader1.setOptions(optsR);
    Stage& reader2 = mgr.addReader("readers.las");
    reader2.setOptions(optsR);

    Options optsF;
    optsF.add("step", 3);
    optsF.add("offset", 1);
    Stage& filter = mgr.addFilter("filters.decimation");
    filter.setInput(reader1);
    filter.setOptions(optsF);

    Options optsW;
    optsW.add("filename", outfile, "file to write to");
    Stage& writer = mgr.addWriter("writers.las");
    writer.setInput(filter);
    writer.setInput(reader2);
    writer.setOptions(optsW);

    std::cout << "E" << std::endl;
    point_count_t np = mgr.execute();
    std::cout << "Done" << std::endl;
    EXPECT_EQ(1420U, np);

    EXPECT_TRUE(!std::ifstream(outfile).fail());
}

TEST(AdditionalMergeTest, reader_and_filter_writer_inputs_with_manager)
{
    const char * outfile = "reader_and_filter_writer_inputs_with_manager.las";
    FileUtils::deleteFile(outfile);

    PipelineManager mgr;

    Options optsR;
    optsR.add("filename", Support::datapath("las/1.2-with-color.las"));
    Stage& reader1 = mgr.addReader("readers.las");
    reader1.setOptions(optsR);
    Stage& reader2 = mgr.addReader("readers.las");
    reader2.setOptions(optsR);

    Options optsF;
    optsF.add("step", 3);
    optsF.add("offset", 1);
    Stage& filter = mgr.addFilter("filters.decimation");
    filter.setInput(reader1);
    filter.setOptions(optsF);

    Options optsW;
    optsW.add("filename", outfile, "file to write to");
    Stage& writer = mgr.addWriter("writers.las");
    writer.setInput(reader2);
    writer.setInput(filter);
    writer.setOptions(optsW);

    std::cout << "E" << std::endl;
    point_count_t np = mgr.execute();
    std::cout << "Done" << std::endl;
    EXPECT_EQ(1420U, np);

    EXPECT_TRUE(!std::ifstream(outfile).fail());
}

TEST(AdditionalMergeTest, filter_and_reader_writer_inputs_without_manager)
{
    const char * outfile = "filter_and_reader_writer_inputs_without_manager.las";
    FileUtils::deleteFile(outfile);

    Options optsR;
    optsR.add("filename", Support::datapath("las/1.2-with-color.las"));
    LasReader reader1;
    reader1.setOptions(optsR);
    LasReader reader2;
    reader2.setOptions(optsR);

    Options optsF;
    optsF.add("step", 3);
    optsF.add("offset", 1);
    DecimationFilter filter;
    filter.setInput(reader1);
    filter.setOptions(optsF);

    Options optsW;
    optsW.add("filename", outfile, "file to write to");
    LasWriter writer;
    writer.setInput(filter);
    writer.setInput(reader2);
    writer.setOptions(optsW);

    PointTable table;
    writer.prepare(table);

    std::cout << "E" << std::endl;
    PointViewSet vs = writer.execute(table);
    EXPECT_EQ(1u, vs.size());
    point_count_t np = (*vs.begin())->size();
    std::cout << "Done" << std::endl;
    EXPECT_EQ(1420U, np);

    EXPECT_TRUE(!std::ifstream(outfile).fail());
}

TEST(AdditionalMergeTest, reader_and_filter_writer_inputs_without_manager)
{
    const char * outfile = "reader_and_filter_writer_inputs_without_manager.las";
    FileUtils::deleteFile(outfile);

    Options optsR;
    optsR.add("filename", Support::datapath("las/1.2-with-color.las"));
    LasReader reader1;
    reader1.setOptions(optsR);
    LasReader reader2;
    reader2.setOptions(optsR);

    Options optsF;
    optsF.add("step", 3);
    optsF.add("offset", 1);
    DecimationFilter filter;
    filter.setInput(reader1);
    filter.setOptions(optsF);

    Options optsW;
    optsW.add("filename", outfile, "file to write to");
    LasWriter writer;
    writer.setInput(reader2);
    writer.setInput(filter);
    writer.setOptions(optsW);

    PointTable table;
    writer.prepare(table);

    std::cout << "E" << std::endl;
    PointViewSet vs = writer.execute(table);
    EXPECT_EQ(1u, vs.size());
    point_count_t np = (*vs.begin())->size();
    std::cout << "Done" << std::endl;
    EXPECT_EQ(1420U, np);

    EXPECT_TRUE(!std::ifstream(outfile).fail());
}
