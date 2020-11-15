#include "gtest/gtest.h"
#include "DetectTrackFace.hpp"

class DetectTrackFaceTest : public ::testing::Test
{
public:
    DetectTrackFaceTest()
        : m_controlStep(), m_bodyTrackEnabled(false), m_bodyDetected(false), m_detectTrackFace()
    {
    }

protected:
    Step m_controlStep;
    bool m_bodyTrackEnabled;
    bool m_bodyDetected;

    DetectTrackFace m_detectTrackFace;
};

TEST_F(DetectTrackFaceTest, detectBodyTC1BodyDetectedOnRightTrackDisabled)
{
    // GIVEN
    m_controlStep.step_up = 0;
    m_controlStep.step_down = 0;
    m_controlStep.control_active = 0;
    m_bodyTrackEnabled = false;
    m_bodyDetected = false;
    Mat imageInput = imread("../test/testinput/personPresentOnRight.png");
    m_detectTrackFace.createFullBodyDetector();

    // WHEN
    Mat dst, gray;
    resize(imageInput, dst, Size(220, 160)); //change the size of the frame
    cvtColor(dst, gray, COLOR_BGR2GRAY);     //convert to gray
    m_detectTrackFace.loadGrafic(gray, dst);
    Mat output = m_detectTrackFace.detectBody(m_controlStep, m_bodyTrackEnabled, m_bodyDetected);

    // THEN
    EXPECT_TRUE(m_bodyDetected) << "TC1: m_bodyDetected test failed";
    EXPECT_FALSE(m_controlStep.control_active) << "TC1: m_controlStep.control_active test failed";
    EXPECT_EQ(0, m_controlStep.step_up) << "TC1: m_controlStep.step_up test failed";
    EXPECT_EQ(0, m_controlStep.step_down) << "TC1: m_controlStep.step_down test failed";
}

TEST_F(DetectTrackFaceTest, detectBodyTC2BodyDetectedOnRightTrackEnabled)
{
    // GIVEN
    m_controlStep.step_up = 0;
    m_controlStep.step_down = 0;
    m_controlStep.control_active = 0;
    m_bodyTrackEnabled = true;
    m_bodyDetected = false;
    Mat imageInput = imread("../test/testinput/personPresentOnRight.png");
    m_detectTrackFace.createFullBodyDetector();

    // WHEN
    Mat dst, gray;
    resize(imageInput, dst, Size(220, 160)); //change the size of the frame
    cvtColor(dst, gray, COLOR_BGR2GRAY);     //convert to gray
    m_detectTrackFace.loadGrafic(gray, dst);
    m_detectTrackFace.detectBody(m_controlStep, m_bodyTrackEnabled, m_bodyDetected);

    // THEN
    EXPECT_TRUE(m_bodyDetected) << "TC2: m_bodyDetected test failed";
    EXPECT_TRUE(m_controlStep.control_active) << "TC2: m_controlStep.control_active test failed";
    EXPECT_EQ(-1, m_controlStep.step_up) << "TC2: m_controlStep.step_up test failed";
    EXPECT_EQ(-1, m_controlStep.step_down) << "TC2: m_controlStep.step_down test failed";
}

TEST_F(DetectTrackFaceTest, detectBodyTC3BodyDetectedOnLeftTrackEnabled)
{
    // GIVEN
    m_controlStep.step_up = 0;
    m_controlStep.step_down = 0;
    m_controlStep.control_active = 0;
    m_bodyTrackEnabled = true;
    m_bodyDetected = false;
    Mat imageInput = imread("../test/testinput/personPresentOnLeft.png");
    m_detectTrackFace.createFullBodyDetector();

    // WHEN
    Mat dst, gray;
    resize(imageInput, dst, Size(220, 160)); //change the size of the frame
    cvtColor(dst, gray, COLOR_BGR2GRAY);     //convert to gray
    m_detectTrackFace.loadGrafic(gray, dst);
    m_detectTrackFace.detectBody(m_controlStep, m_bodyTrackEnabled, m_bodyDetected);

    // THEN
    EXPECT_TRUE(m_bodyDetected) << "TC3: m_bodyDetected test failed";
    EXPECT_TRUE(m_controlStep.control_active) << "TC3: m_controlStep.control_active test failed";
    EXPECT_EQ(-1, m_controlStep.step_up) << "TC3: m_controlStep.step_up test failed";
    EXPECT_EQ(1, m_controlStep.step_down) << "TC3: m_controlStep.step_down test failed";
}

TEST_F(DetectTrackFaceTest, detectBodyTC4BodyNotDetectedTrackEnabled)
{
    // GIVEN
    m_controlStep.step_up = 0;
    m_controlStep.step_down = 0;
    m_controlStep.control_active = 0;
    m_bodyTrackEnabled = true;
    m_bodyDetected = false;
    Mat imageInput = imread("../test/testinput/noPersonPresent.png");
    m_detectTrackFace.createFullBodyDetector();

    // WHEN
    Mat dst, gray;
    resize(imageInput, dst, Size(220, 160)); //change the size of the frame
    cvtColor(dst, gray, COLOR_BGR2GRAY);     //convert to gray
    m_detectTrackFace.loadGrafic(gray, dst);
    m_detectTrackFace.detectBody(m_controlStep, m_bodyTrackEnabled, m_bodyDetected);

    // THEN
    EXPECT_FALSE(m_bodyDetected) << "TC4: m_bodyDetected test failed";
    EXPECT_FALSE(m_controlStep.control_active) << "TC4: m_controlStep.control_active test failed";
    EXPECT_EQ(0, m_controlStep.step_up) << "TC4: m_controlStep.step_up test failed";
    EXPECT_EQ(0, m_controlStep.step_down) << "TC4: m_controlStep.step_down test failed";
}
