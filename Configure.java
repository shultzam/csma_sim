// Java program to implement a simple DataEntry Form using Java Swing.

import java.io.PrintWriter;
import java.awt.*;
import java.awt.event.*;
import javax.swing.*;

class MyFrame extends JFrame implements ActionListener {
   // INI File path.
   private String INI_FILE = "./csma_config.ini";
   
	// Components of the Form.
   private Container container;
   private JCheckBox cbVerboseEnabled;
   private JTextField tbSimulationCount;
   private JTextField tbTimeSlots;
   private JComboBox<String> ddProtocolTypes;
   private JTextField tbProbPersistence;
   private JTextField tbNodeCount;
   private JTextField tbProbFrameGeneration;
   private JTextField tbFrameLength;
   private JTextField tbBackoffMaxRetransmitCount;
   private JButton submitButton;
   private JButton resetButton;
   
   // Default settings.
   private String DEFAULT_SIM_COUNT = "100";
   private String DEFAULT_TIME_SLOT_COUNT = "1000000";
   private Integer DEFAULT_PROTOCOL_TYPE = 1;
   private String DEFAULT_PROB_PERSISTENCE = "0.1";
   private String DEFAULT_NODE_COUNT = "6";
   private String DEFAULT_PROB_FRAME_GENERATION = "0.1";
   private String DEFAULT_FRAME_LENGTH = "10";
   private String DEFAULT_MAX_RETRANSMIT_COUNT = "10";
   
   private String protocolTypes[] = {
      "Non-Persistent",
      "1-Persistent",
      "p-Persistent"
   };

   // Constructor - initializes the components with default values.
   public MyFrame() {
      // Initialize the form.
      setLocationByPlatform(true);

      // Save instance of the JFrame to a variable.
		container = getContentPane();   
      
      // Initialize a JLabel for the title of the GUI.
      JLabel lInstructions = new JLabel("Please select the configuration values you would like to test. The fields are pre-populated with defaults for your convenience.");
      container.add(lInstructions, "North");
      
      // Initialize a new panel for the dropdown menu.
      JPanel northPanel = new JPanel();
      northPanel.setLayout(new GridLayout(0, 2, 5, 10));
      
      // Initialize a JLable and checkbox allowing the user to enable/disable verbose logging.
      JLabel lVerboseEnabled = addJLabel("Toggle to enable verbose logging:");
      northPanel.add(lVerboseEnabled);
      cbVerboseEnabled = new JCheckBox();
      northPanel.add(cbVerboseEnabled);
      
      // Initialize a JLabel and textbox for the total time slots for the simulation.
      JLabel lSimCount = addJLabel("Simulations to execute [1, 100]:");
      northPanel.add(lSimCount);
      tbSimulationCount = addJTextField(DEFAULT_SIM_COUNT);
      northPanel.add(tbSimulationCount);
      
      // Initialize a JLabel and textbox for the total time slots for the simulation.
      JLabel lTimeSlots = addJLabel("Total time slots for each simulation [1, 2500000]:");
      northPanel.add(lTimeSlots);
      tbTimeSlots = addJTextField(DEFAULT_TIME_SLOT_COUNT);
      northPanel.add(tbTimeSlots);
      
      // Initialize a JLabel for the protocol dropdown menu.
      JLabel lProtocolTypes = addJLabel("Select a protocol:");
      northPanel.add(lProtocolTypes);
      
      // Initialize the protocol dropdown menu.
      ddProtocolTypes = new JComboBox<String>(protocolTypes);
      ddProtocolTypes.setMaximumSize(ddProtocolTypes.getPreferredSize());
      ddProtocolTypes.setSelectedIndex(DEFAULT_PROTOCOL_TYPE);
      northPanel.add(ddProtocolTypes);
      
      // Initialize a JLabel and textbox for the probability of persistence.
      JLabel lProbPersistence = addJLabel("Probability of Persistence (for p-Persistent only) [0.0, 1.0]:");
      northPanel.add(lProbPersistence);
      tbProbPersistence = addJTextField(DEFAULT_PROB_PERSISTENCE);
      northPanel.add(tbProbPersistence);
      
      // Initialize a JLabel and textbox for the node count.
      JLabel lNodeCount = addJLabel("Node Count [0, 15]:");
      northPanel.add(lNodeCount);
      tbNodeCount = addJTextField(DEFAULT_NODE_COUNT);
      northPanel.add(tbNodeCount);
      
      // Initialize a JLabel and textbox for the probability of frame generation.
      JLabel lFrameGeneration = addJLabel("Probability of Frame Generation [0.0, 1.0]:");
      northPanel.add(lFrameGeneration);
      tbProbFrameGeneration = addJTextField(DEFAULT_PROB_FRAME_GENERATION);
      northPanel.add(tbProbFrameGeneration);
      
      // Initialize a JLabel and textbox for the frame length.
      JLabel lFrameLength = addJLabel("Frame Length (time-slots) [3, 20]:");
      northPanel.add(lFrameLength);
      tbFrameLength = addJTextField(DEFAULT_FRAME_LENGTH);
      northPanel.add(tbFrameLength);
      
      // Initialize a JLabel and textbox for the maximum retransmition count.
      JLabel lBackoffMaxRetransmitCount = addJLabel("Maximum Retransmit Attempts [0, 25]:");
      northPanel.add(lBackoffMaxRetransmitCount);
      tbBackoffMaxRetransmitCount = addJTextField(DEFAULT_MAX_RETRANSMIT_COUNT);
      northPanel.add(tbBackoffMaxRetransmitCount);
      
      // Initialize a third panel for the submit and reset buttons.
      JPanel southPanel = new JPanel(new FlowLayout(FlowLayout.CENTER, 15, 5));

      // Add a submit button.
		submitButton = new JButton("Submit");
		submitButton.setFont(new Font("Arial", Font.PLAIN, 15));
		submitButton.setSize(100, 20);
		submitButton.addActionListener(this);
		southPanel.add(submitButton);

      // Add a reset button.
		resetButton = new JButton("Reset");
		resetButton.setFont(new Font("Arial", Font.PLAIN, 15));
		resetButton.setSize(100, 20);
		resetButton.addActionListener(this);
		southPanel.add(resetButton);
      
      // Add panels to the container.
      container.add(northPanel);
      container.add(southPanel, "South");
      
      // Perform some housekeeping and display the GUI.
		setBounds(75, 75, 980, northPanel.getComponentCount() * 20 + 50);
		setVisible(true);
      setDefaultCloseOperation(EXIT_ON_CLOSE);
	}

	// Method actionPerformed() to get the action performed by the user and act accordingly.
	public void actionPerformed(ActionEvent e) {
		if (e.getSource() == submitButton) {
         // Verify no empty fields. If so, display dialogue and try again.
         if (isStringNullOrEmpty(tbSimulationCount.getText())
          || isStringNullOrEmpty(tbTimeSlots.getText())
          || isStringNullOrEmpty(tbProbPersistence.getText())
          || isStringNullOrEmpty(tbNodeCount.getText())
          || isStringNullOrEmpty(tbProbFrameGeneration.getText())
          || isStringNullOrEmpty(tbFrameLength.getText())
          || isStringNullOrEmpty(tbBackoffMaxRetransmitCount.getText())) {
            JOptionPane.showMessageDialog(this, 
                                          "Failed to create CSMA configuration file due to one or more empty fields.", 
                                          null, 
                                          JOptionPane.ERROR_MESSAGE);
            return;
         }
		            
         // Write the values to file.
         try {            
            // Open the file for writing and write the INI file.
            PrintWriter fileWriter = new PrintWriter(INI_FILE);
            fileWriter.println("[CONFIG]");
            fileWriter.println("VERBOSE_LOGGING=" + cbVerboseEnabled.isSelected());
            fileWriter.println("SIMULATION_COUNT=" + tbSimulationCount.getText());
            fileWriter.println("TIME_SLOT_COUNT=" + tbTimeSlots.getText());
            fileWriter.println("PROTOCOL_TYPE=" + ddProtocolTypes.getSelectedItem());
            fileWriter.println("PROB_PERSISTENCE=" + tbProbPersistence.getText());
            fileWriter.println("NODE_COUNT=" + tbNodeCount.getText());
            fileWriter.println("PROB_FRAME_GENERATION=" + tbProbFrameGeneration.getText());
            fileWriter.println("FRAME_LENGTH=" + tbFrameLength.getText());
            fileWriter.println("MAX_RETRANSMIT_ATTEMPTS=" + tbBackoffMaxRetransmitCount.getText());
            fileWriter.close();
            
            // Signify completion.
            JOptionPane.showMessageDialog(this, 
                                          "Configuration created succesfully (csma_config.ini).", 
                                          null, 
                                          JOptionPane.INFORMATION_MESSAGE);
            
            // Exit the application.
            System.exit(0);
         }
         catch (Exception ex) {
            // Print error.
            JOptionPane.showMessageDialog(this, 
                                          "Failed to create CSMA configuration file.", 
                                          null, 
                                          JOptionPane.ERROR_MESSAGE);
            return;
         }
		}
		else if (e.getSource() == resetButton) {
         // Reset was clicked, reset all values to defaults.
         cbVerboseEnabled.setSelected(false);
         tbSimulationCount.setText(DEFAULT_SIM_COUNT);
         tbTimeSlots.setText(DEFAULT_TIME_SLOT_COUNT);
			ddProtocolTypes.setSelectedIndex(DEFAULT_PROTOCOL_TYPE);
			tbProbPersistence.setText(DEFAULT_PROB_PERSISTENCE);
			tbNodeCount.setText(DEFAULT_NODE_COUNT);
			tbProbFrameGeneration.setText(DEFAULT_PROB_FRAME_GENERATION);
         tbFrameLength.setText(DEFAULT_FRAME_LENGTH);
         tbBackoffMaxRetransmitCount.setText(DEFAULT_MAX_RETRANSMIT_COUNT);
		}
	}
   
   // Method addJLabel() to add a JLabel to the form.
   public JLabel addJLabel(String labelText) {
      JLabel newLabel = new JLabel(labelText);
      newLabel.setAlignmentX(Component.CENTER_ALIGNMENT);
      newLabel.setFont(new Font("Arial", Font.PLAIN, 15));
      return newLabel;
   }
   
   // Method addJTextField() to add a JTextField to the form.
   public JTextField addJTextField(String defaultValue) {
      JTextField newTextField = new JTextField();
      newTextField.setFont(new Font("Arial", Font.PLAIN, 15));
      newTextField.setText(defaultValue);
      return newTextField;
   }
             
   // Method isStringNullOrEmpty() to check if a given string is null or empty.
   public Boolean isStringNullOrEmpty(String inputString) {
      return inputString.trim().isEmpty();
   }
}

// Driver Code.
class Configure {
	public static void main(String[] args) throws Exception {
		MyFrame f = new MyFrame();
	}
}
