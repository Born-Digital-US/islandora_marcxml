<?php
/**
 * @file
 * Adminstration settings for the Islandora MARCXML module.
 */

/**
 * Adminstration form for the Islandora MARCXML module.
 */
function islandora_marcxml_admin_settings(array $form, array &$form_state) {
  $form['islandora_marcxml_allow_editing_of_existing_mods'] = array(
    '#type' => 'checkbox',
    '#title' => t('Allow user to upload a new copy of the MARCXML to be re-transformed and stored as MODS.'),
    '#description' => t("This provides an extra option to be available under the 'edit' link present on objects that have MODS datastreams."),
    '#default_value' => variable_get('islandora_marcxml_allow_editing_of_existing_mods', FALSE),
  );
  $form['islandora_marcxml_select_marc_to_mods_xsl'] = array(
    '#type' => 'select',
    '#title' => t('MARC to MODS XSL file'),
    '#default_value' => variable_get('islandora_marcxml_select_marc_to_mods_xsl', 'MARC21slim2MODS3-6.xsl'),
    '#options' => array(
      'MARC21slim2MODS3-5.xsl' => t('MARCXML to MODS 3.5'),
      'MARC21slim2MODS3-6.xsl' => t('MARCXML to MODS 3.6'),
    ),
    '#description' => t('Select the MARC to MODS XSL file to convert your uploaded MARCXML to MODS.'),
  );
  $form['islandora_marcxml_select_mods_to_marc_xsl'] = array(
    '#type' => 'select',
    '#title' => t('MODS to MARC XSL file'),
    '#default_value' => variable_get('islandora_marcxml_select_mods_to_marc_xsl', 'MODS3-4_MARC21slim_XSLT1-0.xsl'),
    '#options' => array(
      'MODS2MARC21slim.xsl' => t('MODS 3.0 to MARCXML'),
      'MODS3-4_MARC21slim_XSLT1-0.xsl' => t('MODS 3.4 to MARC XSLT 1.0'),
    ),
    '#description' => t('Select the MARC to MODS XSL file to convert your uploaded MARCXML to MODS.'),
  );
  if(module_exists('select_or_other')) {
    $form['islandora_marcxml_select_marc_to_mods_xsl']['#type'] = 'select_or_other';
    $form['islandora_marcxml_select_marc_to_mods_xsl']['#other'] = t('Other (path to file from site root)');
    $form['islandora_marcxml_select_marc_to_mods_xsl']['#other_unknown_defaults'] = 'other';
    $form['islandora_marcxml_select_marc_to_mods_xsl']['#other_delimiter'] = ',';
    $form['islandora_marcxml_select_mods_to_marc_xsl']['#type'] = 'select_or_other';
    $form['islandora_marcxml_select_mods_to_marc_xsl']['#other'] = t('Other (path to file from site root)');
    $form['islandora_marcxml_select_mods_to_marc_xsl']['#other_unknown_defaults'] = 'other';
    $form['islandora_marcxml_select_mods_to_marc_xsl']['#other_delimiter'] = ',';
    $form['#validate'][] = '_islandora_marcxml_admin_settings_select_or_other_validate';
  }
  return system_settings_form($form);
}

/**
 * @param $form
 * @param $form_state
 */
function _islandora_marcxml_admin_settings_select_or_other_validate(&$form, &$form_state) {
  foreach(array('islandora_marcxml_select_marc_to_mods_xsl', 'islandora_marcxml_select_mods_to_marc_xsl') as $field) {
    if(!empty($form_state['values'][$field])) {
      module_load_include('inc', 'islandora_marcxml', 'includes/utilities');
      $file_path = _islandora_marcxml_xslt_file_path($form_state['values'][$field]);
      if(!file_exists($file_path)) {
        form_set_error($field, t('The file entered in @field does not exist or is not accessible.',array('@field' => $field)));
      }
    }
  }
}